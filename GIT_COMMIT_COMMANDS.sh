#!/bin/bash
# 提交代码的命令

echo "准备提交 SQL 关键字修复..."

# 添加修改的文件
git add src/observer/sql/parser/lex_sql.l
git add src/observer/sql/parser/lex_sql.cpp
git add src/observer/sql/parser/lex_sql.h
git add src/observer/sql/parser/yacc_sql.cpp
git add src/observer/sql/parser/yacc_sql.hpp

# 显示将要提交的文件
echo ""
echo "将要提交的文件:"
git status --short

# 提交
echo ""
echo "提交代码..."
git commit -m "修复: 添加所有类型关键字别名支持，修复小写 SQL 语句解析问题

- 添加 INT, FLOAT, STRING, VARCHAR, DATE 关键字别名
- 修复 CREATE TABLE 语句中小写类型关键字被识别为 ID_KEY 的问题
- 确保所有类型关键字都在 ID_KEY 之前定义
- 重新生成解析器文件

修复的问题:
- CREATE TABLE t_basic(id int, age int, name char(4), score float) 解析失败
- 原因: char, int, float 等小写关键字被识别为 ID_KEY
- 解决: 添加关键字别名，使其返回正确的类型 token"

echo ""
echo "✅ 提交完成！"
echo ""
echo "下一步: 推送到远程仓库"
echo "  git push origin local_miniob_2025"
