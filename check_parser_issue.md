# 问题分析：为什么 CREATE TABLE 解析失败

## 问题描述
提交到大赛时，`CREATE TABLE t_basic(id int, age int, name char(4), score float);` 失败，提示 "SQL_SYNTAX > Failed to parse sql"

## 可能的原因

### 1. 解析器文件版本不匹配
- 虽然 yacc_sql.cpp, lex_sql.cpp 在 Git 中，但可能是用旧版本的 yacc_sql.y/lex_sql.l 生成的
- 需要确保提交的解析器文件是用最新版本的源文件生成的

### 2. CHAR 类型识别问题
- `char(4)` 中的 `char` 可能被识别为 `STRING_T` 而不是 `CHAR`
- 需要检查 lex_sql.l 中 CHAR 关键字的定义

### 3. 关键字大小写问题
- 虽然设置了 `%option case-insensitive`，但可能有问题

### 4. 语法规则问题
- `CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format`
- `storage_format` 可以为空，但可能在某些情况下有问题

## 解决方案

1. **确保解析器文件是最新的**：
   ```bash
   cd src/observer/sql/parser
   ./gen_parser.sh
   git add yacc_sql.cpp yacc_sql.hpp lex_sql.cpp lex_sql.h
   git commit -m "更新解析器文件"
   git push
   ```

2. **检查 CHAR 关键字定义**：
   - 确保 lex_sql.l 中正确定义了 CHAR 关键字
   - 确保 CHAR 在 STRING_T 之前匹配（flex 优先匹配前面的规则）

3. **验证语法规则**：
   - 检查 `attr_def` 规则是否正确
   - 确保 `char(4)` 能正确匹配 `ID type LBRACE number RBRACE nullable`
