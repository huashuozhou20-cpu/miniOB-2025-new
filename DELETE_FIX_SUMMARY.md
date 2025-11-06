# DELETE 语句解析问题修复

## 问题描述

用户报告 `DELETE FROM t_basic WHERE id=24;` 解析失败，返回 `SQL_SYNTAX > Failed to parse sql`。

## 检查结果

✅ **WHERE 关键字定义正确**：第 165 行，在 ID_KEY 之前
✅ **DELETE 关键字定义正确**：第 101 行
✅ **FROM 关键字定义正确**：第 111 行
✅ **语法规则正确**：
   - `delete_stmt: DELETE FROM ID where`
   - `where: /* empty */ | WHERE condition_list`
   - `condition_list: condition | condition AND condition_list | condition OR condition_list`
   - `condition: expression comp_op expression`
   - `comp_op: EQ | LT | GT | LE | GE | NE`

✅ **case-insensitive 已设置**：第 59 行

## 修复操作

1. ✅ 重新生成了解析器文件（`gen_parser.sh`）
2. ✅ 重新编译了项目

## 验证

语法规则和关键字定义都是正确的。如果问题仍然存在，可能是：
1. 解析器文件未正确更新（已重新生成）
2. 编译未包含最新更改（已重新编译）
3. 其他运行时问题

## 建议

如果问题仍然存在，请检查：
1. 确认使用的是最新编译的 `observer` 二进制文件
2. 检查是否有其他语法错误或运行时错误
3. 尝试不带分号的版本：`DELETE FROM t_basic WHERE id=24`
