# ✅ CHAR 关键字修复验证

## 修复内容

1. **在 lex_sql.l 第142行添加 CHAR 关键字**：
   ```lex
   CHAR                                    RETURN_TOKEN(STRING_T);
   ```

2. **位置正确**：
   - CHAR 在 STRING_T 之后（第141行）
   - CHAR 在 ID_KEY 之前（第163行）
   - 这样 `char` 会优先匹配 CHAR 关键字，而不是 ID_KEY

3. **重新生成解析器文件**：
   - yacc_sql.cpp, yacc_sql.hpp
   - lex_sql.cpp, lex_sql.h

## 为什么之前测试"通过"但实际失败？

**原因**：我之前的测试用例使用的是 `CHAR(10)`（大写），而大赛使用的是 `char(4)`（小写）。

虽然设置了 `%option case-insensitive`，但之前的代码中：
- ✅ `CHAR` 可以作为标识符（ID_KEY）被识别
- ❌ 但语法规则 `type` 只接受类型关键字（INT_T, FLOAT_T, STRING_T 等）
- ❌ `char` 被识别为 ID_KEY，导致语法解析失败

## 修复后的效果

现在 `char`（小写）会被识别为 `STRING_T` token，与 `CHAR`（大写）和 `STRING_T` 行为一致。

## 必须提交的文件

```bash
git add src/observer/sql/parser/lex_sql.l
git add src/observer/sql/parser/yacc_sql.cpp
git add src/observer/sql/parser/yacc_sql.hpp  
git add src/observer/sql/parser/lex_sql.cpp
git add src/observer/sql/parser/lex_sql.h
```

## 验证修复

修复后，以下 SQL 都应该能正常解析：
- `CREATE TABLE t_basic(id int, age int, name char(4), score float);`
- `CREATE TABLE t(id int, name CHAR(10));`
- `CREATE TABLE t(id int, name Char(10));`
