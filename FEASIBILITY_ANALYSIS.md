# MiniOB 功能实现可行性分析

## ✅ 已实现的功能（13项）

1. drop-table ✓
2. update ✓
3. date ✓
4. join-tables ✓（已修复）
5. expression ✓
6. multi-index ✓
7. group-by ✓
8. simple-sub-query ✓
9. alias ✓
10. null ✓
11. order-by ✓
12. text ✓
13. update-mvcc ✓

## 🔨 可实现的功能（按难度排序）

### 1. function（难度：⭐⭐）
**目标**：实现 `LENGTH`、`ROUND`、`DATE_FORMAT` 函数

**可行性**：✅ **高**
- 已有 `SysFuncExpr` 框架
- `DATE_FORMAT` 已有部分实现（在 yacc_sql.y 中）
- 需要：
  - 完善 `SysFuncExpr` 的类型检查
  - 实现 `LENGTH`、`ROUND` 函数逻辑
  - 完善 `DATE_FORMAT` 的格式符处理

**实现位置**：
- `src/observer/sql/expr/sysfunc_expr.cpp`
- `src/observer/sql/expr/sysfunc_expr.h`
- `src/observer/sql/parser/yacc_sql.y`（已有部分代码）

---

### 2. unique（难度：⭐⭐⭐）
**目标**：实现唯一索引

**可行性**：✅ **高**
- B+Tree 已实现
- 需要：
  - 在索引插入时检查重复键
  - 添加 `UNIQUE` 关键字解析
  - 元数据中标记索引为唯一
  - 在 INSERT/UPDATE 时检查唯一性约束

**实现位置**：
- `src/observer/storage/index/bplus_tree.cpp`
- `src/observer/sql/stmt/create_index_stmt.cpp`
- `src/observer/sql/parser/yacc_sql.y`

---

### 3. union（难度：⭐⭐⭐）
**目标**：实现 UNION 和 UNION ALL

**可行性**：✅ **中等**
- 需要：
  - 扩展 SELECT 语句解析支持 UNION
  - 实现结果集去重逻辑（UNION）
  - 实现结果集合并逻辑（UNION ALL）
  - 类型和列数检查

**实现位置**：
- `src/observer/sql/parser/yacc_sql.y`
- `src/observer/sql/optimizer/logical_plan_generator.cpp`
- `src/observer/sql/executor/union_executor.cpp`（新建）

---

### 4. vector-basic（难度：⭐⭐⭐）
**目标**：向量类型基础功能

**可行性**：✅ **高**（我们已修复语法解析）
- ✅ 语法解析已修复（VECTOR(维度)）
- 需要：
  - 实现向量存储（`std::vector<double>`）
  - 实现 `DISTANCE` 函数（Cosine/Euclidean/Dot）
  - 实现 `VECTOR_TO_STRING` 和 `STRING_TO_VECTOR`
  - 向量格式化（科学计数法）

**实现位置**：
- `src/observer/common/type/vector_type.cpp`（已有部分）
- `src/observer/sql/expr/sysfunc_expr.cpp`
- `src/observer/storage/record/record_manager.cpp`（向量存储）

---

### 5. alter（难度：⭐⭐⭐⭐）
**目标**：ALTER TABLE 四种操作

**可行性**：✅ **中等**
- 需要：
  - ADD COLUMN：添加列到表结构
  - DROP COLUMN：删除列和数据
  - CHANGE COLUMN：重命名列
  - RENAME TABLE：重命名表（已部分支持）
  - 更新索引元数据

**实现位置**：
- `src/observer/sql/stmt/alter_table_stmt.cpp`
- `src/observer/storage/table/table.cpp`
- `src/observer/storage/index/bplus_tree.cpp`（索引更新）

---

### 6. vector-search（难度：⭐⭐⭐）
**目标**：向量精确检索

**可行性**：✅ **高**（依赖 vector-basic）
- 需要：
  - 实现基于距离的排序
  - 支持 LIMIT 子句
  - 优化性能（避免全表扫描排序）

**实现位置**：
- `src/observer/sql/executor/select_executor.cpp`
- `src/observer/sql/optimizer/physical_plan_generator.cpp`

---

### 7. complex-sub-query（难度：⭐⭐⭐⭐）
**目标**：关联子查询

**可行性**：⚠️ **较难**
- 需要：
  - 外层查询的字段绑定到子查询
  - EXISTS/NOT EXISTS 实现
  - 嵌套查询优化

**实现位置**：
- `src/observer/sql/stmt/select_stmt.cpp`
- `src/observer/sql/optimizer/logical_plan_generator.cpp`
- `src/observer/sql/executor/subquery_executor.cpp`（新建）

---

### 8. create-view（难度：⭐⭐⭐⭐）
**目标**：视图创建和更新

**可行性**：⚠️ **较难**
- 需要：
  - 存储视图定义（SELECT AST）
  - 查询时展开视图
  - 视图更新规则（可更新性检查）

**实现位置**：
- `src/observer/storage/table/view.cpp`（已有部分）
- `src/observer/sql/stmt/create_view_stmt.cpp`
- `src/observer/sql/optimizer/logical_plan_generator.cpp`

---

### 9. full-text-index（难度：⭐⭐⭐⭐⭐）
**目标**：全文检索

**可行性**：⚠️ **困难**
- 需要：
  - 集成 `cppjieba` 中文分词
  - 实现倒排索引
  - 实现 BM25 排序算法
  - 词库管理

**实现位置**：
- `src/observer/storage/index/fulltext_index.cpp`（新建）
- `src/observer/sql/expr/sysfunc_expr.cpp`（TOKENIZE, MATCH, AGAINST）

---

### 10. big-order-by（难度：⭐⭐⭐⭐⭐）
**目标**：外部排序

**可行性**：⚠️ **困难**
- 需要：
  - 实现分块排序
  - 实现 K 路归并
  - 磁盘 I/O 优化
  - 内存限制管理（350MB）

**实现位置**：
- `src/observer/sql/executor/external_sort_executor.cpp`（新建）
- `src/observer/storage/common/external_sort.cpp`（新建）

---

## 📊 实现优先级建议

### 第一阶段（相对简单，1-2周）
1. ✅ **function** - 函数实现（LENGTH, ROUND, DATE_FORMAT）
2. ✅ **vector-basic** - 向量基础（语法已修复，实现存储和函数）
3. ✅ **unique** - 唯一索引（B+Tree 基础上扩展）

### 第二阶段（中等难度，2-3周）
4. ✅ **union** - UNION 和 UNION ALL
5. ✅ **vector-search** - 向量检索（依赖 vector-basic）
6. ✅ **alter** - ALTER TABLE 操作

### 第三阶段（较难，3-4周）
7. ⚠️ **complex-sub-query** - 关联子查询
8. ⚠️ **create-view** - 视图功能

### 第四阶段（困难，4-6周）
9. ⚠️ **full-text-index** - 全文检索（需要外部依赖）
10. ⚠️ **big-order-by** - 外部排序（性能优化）

---

## 🎯 总结

**可以实现的**：✅ 10项功能都可以实现

**推荐先实现**：
1. function（最简单，已有框架）
2. vector-basic（语法已修复，继续完善）
3. unique（B+Tree 基础上扩展）

**需要特别注意**：
- full-text-index 需要集成 cppjieba，需要确保环境支持
- big-order-by 需要仔细设计内存管理
- create-view 和 complex-sub-query 需要深入理解查询优化

所有功能都有实现路径，关键是按照优先级逐步实现！

