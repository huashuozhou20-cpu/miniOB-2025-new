/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"
#include "sql/operator/union_logical_operator.h"

#include <common/log/log.h>
#include <set>
#include <unordered_set>

#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/vector_index_get_logical_operator.h"
#include "sql/operator/group_by_logical_operator.h"
#include "sql/operator/order_by_logical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/create_table_logical_operator.h"
#include "sql/operator/limit_logical_operator.h"

#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/update_stmt.h"
#include "sql/stmt/create_table_stmt.h"
#include "sql/stmt/stmt.h"

#include "sql/expr/expression_iterator.h"
#include "sql/expr/expression.h"
#include "storage/index/index.h"
#include "storage/index/ivfflat_index.h"
#include <cstring>

using namespace std;
using namespace common;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
    case StmtType::CREATE_TABLE: {
      CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);

      rc = create_plan(create_table_stmt, logical_operator);
    } break;

    case StmtType::CALC: {
      CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);

      rc = create_plan(calc_stmt, logical_operator);
    } break;

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);

      rc = create_plan(select_stmt, logical_operator);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);

      rc = create_plan(insert_stmt, logical_operator);
    } break;

    case StmtType::UPDATE: {
      UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);

      rc = create_plan(update_stmt, logical_operator);
    } break;

    case StmtType::DELETE: {
      DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);

      rc = create_plan(delete_stmt, logical_operator);
    } break;

    case StmtType::EXPLAIN: {
      ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);

      rc = create_plan(explain_stmt, logical_operator);
    } break;
    default: {
      rc = RC::UNIMPLEMENTED;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(CreateTableStmt *create_table_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  std::unique_ptr<LogicalOperator> select_oper;
  Stmt *create_select_stmt =  create_table_stmt->select_stmt().get();
  if(create_select_stmt == nullptr)return RC::UNIMPLEMENTED;
  if (nullptr != create_select_stmt) {
    SelectStmt *select_stmt = static_cast<SelectStmt*>(create_select_stmt);
    rc = create_plan(select_stmt, select_oper);
    if (RC::SUCCESS != rc) {
      LOG_WARN("failed to create sub select logical plan, r=%s", strrc(rc));
      return rc;
    }
  }

  auto& table_name = create_table_stmt->table_name();
  auto& attr_infos = create_table_stmt->attr_infos();
  logical_operator = std::unique_ptr<LogicalOperator>(new CreateTableLogicalOperator(
      create_table_stmt->db(), move(table_name), move(attr_infos), create_table_stmt->storage_format())); 
  if (select_oper) {
    logical_operator->add_child(std::move(select_oper));
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  if(OB_SUCC(create_vector_plan(select_stmt, logical_operator)))return RC::SUCCESS;

  unique_ptr<LogicalOperator> *last_oper = nullptr;

  unique_ptr<LogicalOperator> table_oper(nullptr);
  last_oper = &table_oper;

  auto &tables = select_stmt->tables();
  
  FilterStmt *filter_stmt = select_stmt->filter_stmt();
  
  // 提取 JOIN 条件和 WHERE 条件（仅用于多表查询）
  std::vector<unique_ptr<Expression>> join_conditions;
  std::vector<unique_ptr<Expression>> where_conditions;
  
  if (filter_stmt != nullptr && tables.size() > 1) {
    // 多表情况下，需要分离 JOIN 条件和 WHERE 条件
    auto &filter_units = filter_stmt->filter_units();
    for (auto& expr : filter_units) {
      if (expr == nullptr) {
        continue; // 跳过无效表达式
      }
      
      // 检查条件是否涉及两个表（JOIN 条件）
      bool is_join_condition = false;
      if (expr->type() == ExprType::COMPARISON) {
        ComparisonExpr *cmp_expr = static_cast<ComparisonExpr*>(expr.get());
        auto& left = cmp_expr->left();
        auto& right = cmp_expr->right();
        
        // 提取左右表达式涉及的表名
        std::set<std::string> left_tables;
        std::set<std::string> right_tables;
        
        auto collect_tables = [&](Expression *e, std::set<std::string> &tbls) {
          if (e->type() == ExprType::FIELD) {
            FieldExpr *field_expr = static_cast<FieldExpr*>(e);
            const BaseTable *tbl = field_expr->table();
            if (tbl != nullptr) {
              tbls.insert(tbl->name());
              const std::string &alias = field_expr->table_alias();
              if (!alias.empty()) {
                tbls.insert(alias);
              }
            }
          } else {
            // 递归检查子表达式
            ExpressionIterator::iterate_child_expr(*e, [&](std::unique_ptr<Expression> &child) {
              if (child->type() == ExprType::FIELD) {
                FieldExpr *field_expr = static_cast<FieldExpr*>(child.get());
                const BaseTable *tbl = field_expr->table();
                if (tbl != nullptr) {
                  tbls.insert(tbl->name());
                  const std::string &alias = field_expr->table_alias();
                  if (!alias.empty()) {
                    tbls.insert(alias);
                  }
                }
              }
              return RC::SUCCESS;
            });
          }
          return RC::SUCCESS;
        };
        
        collect_tables(left.get(), left_tables);
        collect_tables(right.get(), right_tables);
        
        // JOIN 条件：左右都涉及表，且是不同的表
        if (!left_tables.empty() && !right_tables.empty()) {
          // 检查是否有交集（如果左右涉及同一个表，则不是 JOIN 条件）
          bool has_common_table = false;
          for (const auto &t : left_tables) {
            if (right_tables.find(t) != right_tables.end()) {
              has_common_table = true;
              break;
            }
          }
          
          if (!has_common_table) {
            is_join_condition = true;
          }
        }
      }
      
      if (is_join_condition) {
        join_conditions.emplace_back(std::move(expr));
      } else {
        where_conditions.emplace_back(std::move(expr));
      }
    }
    // 注意：这里我们移动了 filter_units 中的所有表达式，原始的 filter_stmt 会被清空
    // 但这是预期的，因为我们在后续会使用分离后的 join_conditions 和 where_conditions
  }
  // 单表情况下，保持原逻辑，直接使用 filter_stmt
  
  // 创建表算子
  size_t table_idx = 0;
  for (auto& [table, alias] : tables) {    
    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_ONLY, alias));
    if (table_oper == nullptr) {
      table_oper = std::move(table_get_oper);
    } else {
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      
      // 设置 JOIN 条件：提取涉及当前两个表的条件
      if (table_idx == 1 && !join_conditions.empty()) {
        // 对于第一个 JOIN，使用所有 JOIN 条件
        if (join_conditions.size() == 1) {
          join_oper->set_join_condition(std::move(join_conditions[0]));
        } else {
          unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::AND, join_conditions));
          join_oper->set_join_condition(std::move(conjunction_expr));
        }
      }
      
      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(table_get_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
    table_idx++;
  }

  unique_ptr<LogicalOperator> predicate_oper;

  // 创建 WHERE 条件的 FilterStmt
  RC rc = RC::SUCCESS;
  if (tables.size() > 1) {
    // 多表情况下
    if (!where_conditions.empty()) {
      // 有 WHERE 条件，使用分离后的 WHERE 条件
      FilterStmt *where_filter_stmt = new FilterStmt(filter_stmt ? filter_stmt->and_or() : false);
      where_filter_stmt->filter_units().swap(where_conditions);
      rc = create_plan(where_filter_stmt, predicate_oper);
      delete where_filter_stmt;
      if (OB_FAIL(rc)) {
        LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
        return rc;
      }
    }
    // 如果没有 WHERE 条件（所有条件都是 JOIN 条件），则不创建 predicate_oper
  } else if (filter_stmt != nullptr) {
    // 单表情况下，直接使用原始的 filter_stmt
    rc = create_plan(filter_stmt, predicate_oper);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
      return rc;
    }
  }

  if (predicate_oper) {
    if (*last_oper) {
      predicate_oper->add_child(std::move(*last_oper));
    }

    last_oper = &predicate_oper;
  }

  unique_ptr<LogicalOperator> group_by_oper;
  rc = create_group_by_plan(select_stmt, group_by_oper);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create group by logical plan. rc=%s", strrc(rc));
    return rc;
  }

  if (group_by_oper) {
    if (*last_oper) {
      group_by_oper->add_child(std::move(*last_oper));
    }

    last_oper = &group_by_oper;
  }

  unique_ptr<LogicalOperator> having_oper;
  if (!select_stmt->having_list().empty()) {
    if(select_stmt->and_or()){
      unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::OR, select_stmt->having_list()));
      having_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr)));
    }else{
      unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::AND, select_stmt->having_list()));
      having_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr)));
    }

    if (*last_oper) {
      having_oper->add_child(std::move(*last_oper));
    }
    last_oper = &having_oper;
  }

  auto& order_by = select_stmt->order_by();
  unique_ptr<LogicalOperator> order_by_oper;
  if(order_by.size()){
    order_by_oper = make_unique<OrderByLogicalOperator>(std::move(order_by), std::move(select_stmt->is_asc()), select_stmt->limit());
    if (*last_oper) {
      order_by_oper->add_child(std::move(*last_oper));
    }
    
    last_oper = &order_by_oper;
  }

  unique_ptr<LogicalOperator> limit_oper;
  if(select_stmt->limit() > 0){
    limit_oper = make_unique<LimitLogicalOperator>();
    if (*last_oper) {
      limit_oper->add_child(std::move(*last_oper));
    }
    
    last_oper = &limit_oper;
  }

  auto project_oper = make_unique<ProjectLogicalOperator>(std::move(select_stmt->query_expressions()), tables.size() > 1);
  if (*last_oper) {
    project_oper->add_child(std::move(*last_oper));
  }

  // 处理 UNION
  if (select_stmt->union_stmt() != nullptr) {
    unique_ptr<LogicalOperator> union_left_oper = std::move(project_oper);
    
    // 为 UNION 的右子树创建逻辑计划
    unique_ptr<LogicalOperator> union_right_oper;
    rc = create_plan(select_stmt->union_stmt(), union_right_oper);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to create union right logical plan. rc=%s", strrc(rc));
      return rc;
    }

    // 创建 UNION 逻辑算子
    unique_ptr<UnionLogicalOperator> union_oper(new UnionLogicalOperator(select_stmt->union_all()));
    union_oper->add_child(std::move(union_left_oper));
    union_oper->add_child(std::move(union_right_oper));
    
    logical_operator = std::move(union_oper);
  } else {
    logical_operator = std::move(project_oper);
  }
  
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(FilterStmt *filter_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  if (filter_stmt == nullptr) {
    logical_operator = nullptr;
    return RC::SUCCESS;
  }
  
  RC                                  rc = RC::SUCCESS;
  std::vector<unique_ptr<Expression>> cmp_exprs;
  auto &filter_units = filter_stmt->filter_units();
  
  if (filter_units.empty()) {
    logical_operator = nullptr;
    return RC::SUCCESS;
  }
  
  for (auto& expr : filter_units) {
    if (expr == nullptr) {
      continue; // 跳过已被移动的表达式
    }
    
    ComparisonExpr* cmp_expr = static_cast<ComparisonExpr*>(expr.get());
    if (cmp_expr == nullptr) {
      continue; // 跳过非 ComparisonExpr 类型的表达式
    }
    
    auto& left  = cmp_expr->left();
    auto& right = cmp_expr->right(); 

    if(left->value_type() == AttrType::NULLS || right->value_type() == AttrType::NULLS){
      cmp_exprs.emplace_back(std::move(expr));
      continue;
    }

    if (left->value_type() != right->value_type()) {
      auto left_to_right_cost = implicit_cast_cost(left->value_type(), right->value_type());
      auto right_to_left_cost = implicit_cast_cost(right->value_type(), left->value_type());
      auto left_to_float_cost = implicit_cast_cost(left->value_type(), AttrType::FLOATS);
      auto right_to_float_cost = implicit_cast_cost(right->value_type(), AttrType::FLOATS);

      if(left_to_float_cost != INT32_MAX && right_to_float_cost != INT32_MAX && 
          left_to_float_cost + right_to_float_cost < min(left_to_right_cost, right_to_left_cost)){
        if(left->value_type() != AttrType::FLOATS){
          ExprType left_type = left->type();
          auto cast_expr = make_unique<CastExpr>(std::move(left), AttrType::FLOATS);
          if (left_type == ExprType::VALUE) {
            Value left_val;
            if (OB_FAIL(rc = cast_expr->try_get_value(left_val)))
            {
              LOG_WARN("failed to get value from left child", strrc(rc));
              return rc;
            }
            left = make_unique<ValueExpr>(left_val);
          } else {
            left = std::move(cast_expr);
          }
        }
        if(right->value_type() != AttrType::FLOATS){
          ExprType right_type = right->type();
          auto cast_expr = make_unique<CastExpr>(std::move(right), AttrType::FLOATS);
          if (right_type == ExprType::VALUE) {
            Value right_val;
            if (OB_FAIL(rc = cast_expr->try_get_value(right_val)))
            {
              LOG_WARN("failed to get value from right child", strrc(rc));
              return rc;
            }
            right = make_unique<ValueExpr>(right_val);
          } else {
            right = std::move(cast_expr);
          }
        }
      }
      else if (left_to_right_cost <= right_to_left_cost && left_to_right_cost != INT32_MAX) {
        ExprType left_type = left->type();
        auto cast_expr = make_unique<CastExpr>(std::move(left), right->value_type());
        if (left_type == ExprType::VALUE) {
          Value left_val;
          if (OB_FAIL(rc = cast_expr->try_get_value(left_val)))
          {
            LOG_WARN("failed to get value from left child", strrc(rc));
            return rc;
          }
          left = make_unique<ValueExpr>(left_val);
        } else {
          left = std::move(cast_expr);
        }
      } else if (right_to_left_cost < left_to_right_cost && right_to_left_cost != INT32_MAX) {
        ExprType right_type = right->type();
        auto cast_expr = make_unique<CastExpr>(std::move(right), left->value_type());
        if (right_type == ExprType::VALUE) {
          Value right_val;
          if (OB_FAIL(rc = cast_expr->try_get_value(right_val)))
          {
            LOG_WARN("failed to get value from right child", strrc(rc));
            return rc;
          }
          right = make_unique<ValueExpr>(right_val);
        } else {
          right = std::move(cast_expr);
        }

      } else {
        rc = RC::UNSUPPORTED;
        LOG_WARN("unsupported cast from %s to %s", attr_type_to_string(left->value_type()), attr_type_to_string(right->value_type()));
        return rc;
      }
    }

    ComparisonExpr *new_cmp_expr = new ComparisonExpr(cmp_expr->comp(), std::move(left), std::move(right));
    cmp_exprs.emplace_back(new_cmp_expr);
  }

  unique_ptr<PredicateLogicalOperator> predicate_oper;
  if (!cmp_exprs.empty()) {
    if(filter_stmt->and_or()){
      unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::OR, cmp_exprs));
      predicate_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr)));
    }else{
      unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::AND, cmp_exprs));
      predicate_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr)));
    }
  }

  logical_operator = std::move(predicate_oper);
  return rc;
}

int LogicalPlanGenerator::implicit_cast_cost(AttrType from, AttrType to)
{
  if (from == to) {
    return 0;
  }
  return DataType::type_instance(from)->cast_cost(to);
}

RC LogicalPlanGenerator::create_plan(InsertStmt *insert_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  BaseTable        *table = insert_stmt->table();

  auto values_set = insert_stmt->values_set();
  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, move(values_set));
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(UpdateStmt *update_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  BaseTable                  *table       = update_stmt->table();
  FilterStmt                 *filter_stmt = update_stmt->filter_stmt();
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));

  unique_ptr<LogicalOperator> predicate_oper;

  RC rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  const std::vector<const FieldMeta *>& fields = update_stmt->fields();
  vector<unique_ptr<Expression>>& values = update_stmt->values();

  unique_ptr<LogicalOperator> update_oper(new UpdateLogicalOperator(table, move(fields), move(values)));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    update_oper->add_child(std::move(predicate_oper));
  } else {
    update_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(update_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(DeleteStmt *delete_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  BaseTable                  *table       = delete_stmt->table();
  FilterStmt                 *filter_stmt = delete_stmt->filter_stmt();
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));

  unique_ptr<LogicalOperator> predicate_oper;

  RC rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(ExplainStmt *explain_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> child_oper;

  Stmt *child_stmt = explain_stmt->child();

  RC rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}

RC LogicalPlanGenerator::create_group_by_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  vector<unique_ptr<Expression>> &group_by_expressions = select_stmt->group_by();
  vector<unique_ptr<Expression>> &having_expressions = select_stmt->having_list();
  vector<Expression *> aggregate_expressions;
  vector<unique_ptr<Expression>> &query_expressions = select_stmt->query_expressions();
  function<RC(std::unique_ptr<Expression>&)> collector = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if(expr == nullptr)return rc;
    if (expr->type() == ExprType::AGGREGATION) {
      expr->set_pos(aggregate_expressions.size() + group_by_expressions.size());
      aggregate_expressions.push_back(expr.get());
    }
    rc = ExpressionIterator::iterate_child_expr(*expr, collector);
    return rc;
  };

  function<RC(std::unique_ptr<Expression>&)> bind_group_by_expr = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if(expr == nullptr)return rc;
    for (size_t i = 0; i < group_by_expressions.size(); i++) {
      auto &group_by = group_by_expressions[i];
      if (expr->type() == ExprType::AGGREGATION) {
        break;
      } else if (expr->equal(*group_by)) {
        expr->set_pos(i);
        continue;
      } else {
        rc = ExpressionIterator::iterate_child_expr(*expr, bind_group_by_expr);
      }
    }
    return rc;
  };

 bool found_unbound_column = false;
  function<RC(std::unique_ptr<Expression>&)> find_unbound_column = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if(expr == nullptr)return rc;
    if (expr->type() == ExprType::AGGREGATION) {
      // do nothing
    } else if (expr->pos() != -1) {
      // do nothing
    } else if (expr->type() == ExprType::FIELD) {
      found_unbound_column = true;
    }else {
      rc = ExpressionIterator::iterate_child_expr(*expr, find_unbound_column);
    }
    return rc;
  };
  

  for (unique_ptr<Expression> &expression : query_expressions) {
    bind_group_by_expr(expression);
  }
  for (unique_ptr<Expression> &expression : having_expressions) {
    bind_group_by_expr(expression);
  }

  for (unique_ptr<Expression> &expression : query_expressions) {
    find_unbound_column(expression);
  }
  for (unique_ptr<Expression> &expression : having_expressions) {
    find_unbound_column(expression);
  }

  // collect all aggregate expressions
  for (unique_ptr<Expression> &expression : query_expressions) {
    collector(expression);
  }
  for (unique_ptr<Expression> &expression : having_expressions) {
    collector(expression);
  }

  if (group_by_expressions.empty() && aggregate_expressions.empty()) {
    // 既没有group by也没有聚合函数，不需要group by
    return RC::SUCCESS;
  }

  if (found_unbound_column) {
    LOG_WARN("column must appear in the GROUP BY clause or must be part of an aggregate function");
    return RC::INVALID_ARGUMENT;
  }

  // 如果只需要聚合，但是没有group by 语句，需要生成一个空的group by 语句

  auto group_by_oper = make_unique<GroupByLogicalOperator>(std::move(group_by_expressions),
                                                           std::move(aggregate_expressions));
  logical_operator = std::move(group_by_oper);
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_vector_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  auto &tables = select_stmt->tables();
  auto& order_by = select_stmt->order_by();

  if(tables.size() != 1 || order_by.size() != 1 || !select_stmt->is_asc()[0])
    return RC::INVALID_ARGUMENT;

  Table *table = static_cast<Table*>(tables[0].first);
  Expression *order_expr = order_by[0].get();
  
  VectorOperationExpr::Type operation_type;
  FieldExpr *field_expr = nullptr;
  ValueExpr *value_expr = nullptr;
  Value search_value;  // The vector value to search for
  
  // Check if it's a VectorOperationExpr
  if (order_expr->type() == ExprType::VECTOROPERATION) {
    VectorOperationExpr* vector_expr = static_cast<VectorOperationExpr*>(order_expr);
    
    unique_ptr<Expression> &left_expr  = vector_expr->left();
    unique_ptr<Expression> &right_expr = vector_expr->right();

    // 左右比较的一边最少是一个值
    if (left_expr->type() != ExprType::VALUE && right_expr->type() != ExprType::VALUE) 
      return RC::INVALID_ARGUMENT;

    if (left_expr->type() == ExprType::FIELD) {
      ASSERT(right_expr->type() == ExprType::VALUE, "right expr should be a value expr while left is field expr");
      field_expr = static_cast<FieldExpr *>(left_expr.get());
      value_expr = static_cast<ValueExpr *>(right_expr.get());
    } else if (right_expr->type() == ExprType::FIELD) {
      ASSERT(left_expr->type() == ExprType::VALUE, "left expr should be a value expr while right is a field expr");
      field_expr = static_cast<FieldExpr *>(right_expr.get());
      value_expr = static_cast<ValueExpr *>(left_expr.get());
    }

    if (field_expr == nullptr)return RC::INVALID_ARGUMENT;

    ASSERT(value_expr != nullptr, "got an index but value expr is null ?");
    if (value_expr->try_get_value(search_value) != RC::SUCCESS || search_value.attr_type() != AttrType::VECTORS)
      return RC::INVALID_ARGUMENT;

    operation_type = vector_expr->operation_type();
  } 
  // Check if it's a DISTANCE function (SysFuncExpr)
  else if (order_expr->type() == ExprType::SYSFUNC) {
    SysFuncExpr* sysfunc_expr = static_cast<SysFuncExpr*>(order_expr);
    
    if (sysfunc_expr->sysfunc_type() != SysFuncExpr::Type::DISTANCE) {
      return RC::INVALID_ARGUMENT;
    }
    
    // DISTANCE has 3 arguments: V1 (field), V2 (value), metric (string)
    unique_ptr<Expression> &first_expr = sysfunc_expr->child();
    unique_ptr<Expression> &second_expr = sysfunc_expr->second_child();
    unique_ptr<Expression> &third_expr = sysfunc_expr->third_child();
    
    // Check argument types
    if (first_expr->type() != ExprType::FIELD && first_expr->type() != ExprType::VALUE) {
      return RC::INVALID_ARGUMENT;
    }
    if (second_expr->type() != ExprType::FIELD && second_expr->type() != ExprType::VALUE) {
      return RC::INVALID_ARGUMENT;
    }
    if (third_expr->type() != ExprType::VALUE) {
      return RC::INVALID_ARGUMENT;
    }
    
    // Determine which is field and which is value
    if (first_expr->type() == ExprType::FIELD) {
      field_expr = static_cast<FieldExpr *>(first_expr.get());
      value_expr = static_cast<ValueExpr *>(second_expr.get());
    } else if (second_expr->type() == ExprType::FIELD) {
      field_expr = static_cast<FieldExpr *>(second_expr.get());
      value_expr = static_cast<ValueExpr *>(first_expr.get());
    } else {
      return RC::INVALID_ARGUMENT;  // At least one must be a field
    }
    
    if (field_expr == nullptr || value_expr == nullptr) {
      return RC::INVALID_ARGUMENT;
    }
    
    // Get the value
    if (value_expr->try_get_value(search_value) != RC::SUCCESS || search_value.attr_type() != AttrType::VECTORS) {
      return RC::INVALID_ARGUMENT;
    }
    
    // Get the metric type from third argument
    Value metric_value;
    ValueExpr *metric_expr = static_cast<ValueExpr *>(third_expr.get());
    if (metric_expr->try_get_value(metric_value) != RC::SUCCESS || metric_value.attr_type() != AttrType::CHARS) {
      return RC::INVALID_ARGUMENT;
    }
    
    const char *metric_str = metric_value.data();
    if (metric_str == nullptr) {
      return RC::INVALID_ARGUMENT;
    }
    
    // Map metric string to VectorOperationExpr::Type
    if (0 == strcasecmp(metric_str, "COSINE")) {
      operation_type = VectorOperationExpr::Type::COSINE_DISTANCE;
    } else if (0 == strcasecmp(metric_str, "EUCLIDEAN")) {
      operation_type = VectorOperationExpr::Type::L2_DISTANCE;
    } else if (0 == strcasecmp(metric_str, "DOT")) {
      operation_type = VectorOperationExpr::Type::INNER_PRODUCT;
    } else {
      return RC::INVALID_ARGUMENT;
    }
  } else {
    return RC::INVALID_ARGUMENT;
  }

  Index* index = table->find_index_by_field(field_expr->field().field_name());
  if(index == nullptr || !index->is_vector_index())return RC::INVALID_ARGUMENT;

  if(static_cast<IvfflatIndex*>(index)->type() != operation_type)
    return RC::INVALID_ARGUMENT;
  
  unique_ptr<LogicalOperator> *last_oper = nullptr;
  unique_ptr<LogicalOperator> table_oper(new VectorIndexGetLogicalOperator(index, move(search_value), table, select_stmt->limit()));
  last_oper = &table_oper;

  auto project_oper = make_unique<ProjectLogicalOperator>(std::move(select_stmt->query_expressions()), tables.size() > 1);
  if (*last_oper) {
    project_oper->add_child(std::move(*last_oper));
  }

  logical_operator = std::move(project_oper);
  return RC::SUCCESS;
}