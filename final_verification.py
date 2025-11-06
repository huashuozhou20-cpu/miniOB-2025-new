#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
完整的 MiniOB 功能验证报告
"""

import os
import re

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

def check_file_content(filepath, keywords):
    """检查文件内容和关键字"""
    if not os.path.exists(filepath):
        return False
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            return all(keyword.lower() in content.lower() for keyword in keywords)
    except:
        return False

def verify_function(name, files, keywords_in_files=None, yacc_rule=None):
    """验证功能实现"""
    all_files_exist = all(os.path.exists(os.path.join(SCRIPT_DIR, f)) for f in files)
    
    if not all_files_exist:
        return False, "文件缺失"
    
    if keywords_in_files:
        for filepath, keywords in keywords_in_files.items():
            full_path = os.path.join(SCRIPT_DIR, filepath)
            if not check_file_content(full_path, keywords):
                return False, f"{filepath} 缺少关键字"
    
    if yacc_rule:
        yacc_file = os.path.join(SCRIPT_DIR, "src/observer/sql/parser/yacc_sql.y")
        if os.path.exists(yacc_file):
            with open(yacc_file, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                if not re.search(yacc_rule, content, re.IGNORECASE):
                    return False, "语法规则缺失"
    
    return True, "已实现"

def main():
    print("="*70)
    print("MiniOB 完整功能验证报告")
    print("="*70)
    print()
    
    tasks = {
        "drop-table": ("已过", 
            ["src/observer/sql/stmt/drop_table_stmt.cpp",
             "src/observer/sql/executor/drop_table_executor.cpp"],
            {"src/observer/sql/stmt/drop_table_stmt.cpp": ["drop_table"]},
            r"drop_table_stmt\s*:"),
        
        "update": ("已过",
            ["src/observer/sql/stmt/update_stmt.cpp",
             "src/observer/sql/operator/update_physical_operator.cpp"],
            {"src/observer/sql/operator/update_physical_operator.cpp": ["update_record"]},
            r"update_stmt\s*:"),
        
        "date": ("已过",
            ["src/observer/common/type/date_type.cpp"],
            {"src/observer/common/type/date_type.cpp": ["Date"]},
            r"DATE_T|date_type\s*:"),
        
        "join-tables": ("已过",
            ["src/observer/sql/optimizer/logical_plan_generator.cpp"],
            {"src/observer/sql/optimizer/logical_plan_generator.cpp": ["joined_tables", "JoinLogicalOperator"]},
            r"INNER.*JOIN|join_list"),
        
        "expression": ("已过",
            ["src/observer/sql/expr/expression.cpp"],
            {"src/observer/sql/expr/expression.cpp": ["ArithmeticExpr", "calc_value"]},
            r"expression\s*:|arithmetic"),
        
        "function": ("未过",
            ["src/observer/sql/expr/expression.cpp"],
            {"src/observer/sql/expr/expression.cpp": ["eval_length", "eval_round", "eval_date_format"]},
            r"LENGTH|ROUND|DATE_FORMAT"),
        
        "multi-index": ("已过",
            ["src/observer/storage/index/bplus_tree.cpp"],
            {"src/observer/storage/index/bplus_tree.cpp": ["attr_num"]},
            r"idx_col_list"),
        
        "unique": ("未过",
            ["src/observer/storage/index/bplus_tree.cpp",
             "src/observer/sql/stmt/create_index_stmt.cpp"],
            {"src/observer/storage/index/bplus_tree.cpp": ["unique"]},
            r"UNIQUE|unique_option"),
        
        "group-by": ("已过",
            ["src/observer/sql/optimizer/logical_plan_generator.cpp"],
            {"src/observer/sql/optimizer/logical_plan_generator.cpp": ["GroupBy", "HAVING"]},
            r"GROUP BY|group_by"),
        
        "simple-sub-query": ("已过",
            ["src/observer/sql/stmt/select_stmt.cpp"],
            {"src/observer/sql/stmt/select_stmt.cpp": ["sub_query"]},
            None),
        
        "alias": ("已过",
            ["src/observer/sql/optimizer/logical_plan_generator.cpp"],
            {"src/observer/sql/optimizer/logical_plan_generator.cpp": ["alias"]},
            r"AS\s|alias"),
        
        "null": ("已过",
            ["src/observer/common/value.cpp"],
            {"src/observer/common/value.cpp": ["NULLS", "nullable"]},
            r"NULL_T|null"),
        
        "union": ("未过",
            ["src/observer/sql/operator/union_physical_operator.cpp",
             "src/observer/sql/operator/union_logical_operator.h"],
            {"src/observer/sql/operator/union_physical_operator.cpp": ["union_all", "UnionPhysicalOperator"]},
            r"UNION|union_select"),
        
        "order-by": ("已过",
            ["src/observer/sql/operator/order_by_physical_operator.cpp"],
            {"src/observer/sql/operator/order_by_physical_operator.cpp": ["OrderBy"]},
            r"ORDER BY|order_by"),
        
        "vector-basic": ("未过",
            ["src/observer/common/type/vector_type.cpp",
             "src/observer/sql/expr/expression.cpp"],
            {"src/observer/common/type/vector_type.cpp": ["VectorType"],
             "src/observer/sql/expr/expression.cpp": ["DISTANCE", "VECTOR_TO_STRING", "STRING_TO_VECTOR"]},
            r"VECTOR|DISTANCE"),
        
        "text": ("已过",
            ["src/observer/storage/table/table.cpp"],
            {"src/observer/storage/table/table.cpp": ["TEXT"]},
            r"TEXT_T|TEXT"),
        
        "vector-search": ("未过",
            ["src/observer/sql/expr/expression.cpp"],
            {"src/observer/sql/expr/expression.cpp": ["DISTANCE"]},
            None),
        
        "alter": ("未过",
            ["src/observer/sql/stmt/alter_table_stmt.cpp",
             "src/observer/storage/table/table_alter.cpp"],
            {"src/observer/storage/table/table_alter.cpp": ["ADD_COLUMN", "DROP_COLUMN", "RENAME"]},
            r"ALTER TABLE|alter_table_stmt"),
        
        "update-mvcc": ("已过",
            ["src/observer/storage/trx/mvcc_trx.cpp"],
            {"src/observer/storage/trx/mvcc_trx.cpp": ["update_record"]},
            None),
        
        "complex-sub-query": ("未过",
            ["src/observer/sql/stmt/select_stmt.cpp"],
            {"src/observer/sql/stmt/select_stmt.cpp": ["EXISTS"]},
            r"EXISTS|NOT EXISTS"),
        
        "create-view": ("未过",
            ["src/observer/storage/table/view.cpp",
             "src/observer/sql/stmt/create_view_stmt.cpp"],
            {"src/observer/storage/table/view.cpp": ["View", "create"]},
            r"CREATE VIEW|create_view_stmt"),
        
        "full-text-index": ("未过",
            ["src/observer/storage/index/fulltext_index.cpp"],
            {"src/observer/storage/index/fulltext_index.cpp": ["FullTextIndex", "BM25"]},
            r"FULLTEXT|MATCH.*AGAINST"),
        
        "big-order-by": ("未过",
            ["src/observer/sql/operator/order_by_physical_operator.cpp"],
            {"src/observer/sql/operator/order_by_physical_operator.cpp": ["external_sort", "MEMORY_THRESHOLD"]},
            None),
    }
    
    results = {}
    for name, (status, files, keywords, yacc_rule) in tasks.items():
        implemented, reason = verify_function(name, files, keywords, yacc_rule)
        results[name] = {
            'marked': status,
            'implemented': implemented,
            'reason': reason
        }
    
    # 生成报告
    print("\n" + "="*70)
    print("验证结果")
    print("="*70)
    
    print("\n标记为'已过'的功能:")
    passed_marked = [name for name, info in results.items() if info['marked'] == '已过']
    for name in sorted(passed_marked):
        info = results[name]
        status_icon = "✓" if info['implemented'] else "✗"
        print(f"  {status_icon} {name:20s} 标记:已过  实际: {'已实现' if info['implemented'] else '未实现'} ({info['reason']})")
    
    print("\n标记为'未过'的功能:")
    failed_marked = [name for name, info in results.items() if info['marked'] == '未过']
    for name in sorted(failed_marked):
        info = results[name]
        status_icon = "✓" if info['implemented'] else "✗"
        print(f"  {status_icon} {name:20s} 标记:未过  实际: {'已实现' if info['implemented'] else '未实现'} ({info['reason']})")
    
    # 统计
    implemented = [name for name, info in results.items() if info['implemented']]
    not_implemented = [name for name, info in results.items() if not info['implemented']]
    
    print("\n" + "="*70)
    print("统计")
    print("="*70)
    print(f"总计: {len(results)} 项功能")
    print(f"已实现: {len(implemented)} ({len(implemented)*100//len(results)}%)")
    print(f"未实现: {len(not_implemented)} ({len(not_implemented)*100//len(results)}%)")
    
    print("\n标记为'已过'但实际未实现:")
    for name in sorted(passed_marked):
        if not results[name]['implemented']:
            print(f"  ✗ {name}: {results[name]['reason']}")
    
    print("\n标记为'未过'但实际已实现:")
    for name in sorted(failed_marked):
        if results[name]['implemented']:
            print(f"  ✓ {name}: 代码已实现")

if __name__ == "__main__":
    main()

