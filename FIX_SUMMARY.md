# 🔧 SQL 关键字兼容性修复总结

## 问题

SQL 语句中使用小写类型关键字（如 `int`, `float`, `char`, `string`, `varchar`, `date`）时会被识别为 `ID_KEY`，导致语法解析失败。

## 根本原因

虽然设置了 `%option case-insensitive`，但用户使用的小写关键字（如 `int`）与定义的关键字（如 `INT_T`）不匹配，因为：
- `int` ≠ `INT_T`（下划线不同）
- `float` ≠ `FLOAT_T`
- `string` ≠ `STRING_T`
- `date` ≠ `DATE_T`

## 修复内容

在 `lex_sql.l` 中添加了以下关键字别名：

1. **INT** -> INT_T（第122行）
2. **FLOAT** -> FLOAT_T（第108行）
3. **STRING** -> STRING_T（第142行）
4. **VARCHAR** -> STRING_T（第143行）
5. **DATE** -> DATE_T（第100行）

结合已有的：
- **CHAR** -> STRING_T（第144行，之前已修复）
- **TEXT** -> TEXT_T（第162行，已存在）
- **VECTOR** -> VECTOR_T（第158行，已存在）

## 修复后的效果

现在支持以下所有写法（大小写不敏感）：

- `int` / `INT` / `Int` -> INT_T
- `float` / `FLOAT` / `Float` -> FLOAT_T
- `char` / `CHAR` / `Char` -> STRING_T
- `string` / `STRING` / `String` -> STRING_T
- `varchar` / `VARCHAR` / `Varchar` -> STRING_T
- `date` / `DATE` / `Date` -> DATE_T
- `text` / `TEXT` / `Text` -> TEXT_T
- `vector` / `VECTOR` / `Vector` -> VECTOR_T

## 必须提交的文件

```bash
git add src/observer/sql/parser/lex_sql.l
git add src/observer/sql/parser/yacc_sql.cpp
git add src/observer/sql/parser/yacc_sql.hpp
git add src/observer/sql/parser/lex_sql.cpp
git add src/observer/sql/parser/lex_sql.h
git commit -m "修复: 添加类型关键字别名支持，修复小写 SQL 语句解析问题"
git push
```

## 验证

修复后，以下 SQL 都应该能正常解析：

```sql
CREATE TABLE t1(id int, age int, name char(4), score float);
CREATE TABLE t2(id INT, name VARCHAR(10), birthday DATE);
CREATE TABLE t3(id Int, name String(20), score Float);
CREATE TABLE t4(id int, name varchar(50), date_field date);
```
