# 提交修复说明

## 问题
`CREATE TABLE t_basic(id int, age int, name char(4), score float);` 解析失败

## 根本原因
`lex_sql.l` 中缺少 `CHAR` 关键字定义，导致 `char` 被识别为 `ID_KEY`，而语法规则期望类型关键字。

## 修复
1. 在 `lex_sql.l` 中添加 `CHAR` 关键字，返回 `STRING_T` token
2. 重新生成解析器文件（yacc_sql.cpp, yacc_sql.hpp, lex_sql.cpp, lex_sql.h）

## 提交命令
```bash
git add src/observer/sql/parser/lex_sql.l
git add src/observer/sql/parser/yacc_sql.cpp src/observer/sql/parser/yacc_sql.hpp
git add src/observer/sql/parser/lex_sql.cpp src/observer/sql/parser/lex_sql.h
git commit -m "修复: 添加 CHAR 关键字支持，修复 CREATE TABLE 解析失败问题"
git push
```
