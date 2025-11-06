# 🔧 修复 CREATE TABLE 解析失败问题

## 问题原因

在 `lex_sql.l` 中**缺少 `CHAR` 关键字的定义**！

当用户写 `CREATE TABLE t_basic(id int, age int, name char(4), score float)` 时：
- `int` → 匹配 `INT_T` ✅
- `float` → 匹配 `FLOAT_T` ✅  
- `char` → **没有匹配到任何关键字，被当作 `ID_KEY`** ❌

但语法规则 `type` 只接受：
- `INT_T`
- `FLOAT_T`
- `STRING_T`
- `VECTOR_T` / `VECTOR`
- `TEXT_T`
- `DATE_T`

所以 `char` 被识别为 `ID_KEY`，导致语法解析失败！

## 解决方案

在 `lex_sql.l` 中添加 `CHAR` 关键字，让它返回 `STRING_T` token（因为 `char` 和 `string` 在 MiniOB 中是同一个类型）：

```lex
CHAR                                    RETURN_TOKEN(STRING_T);
```

## 修复步骤

1. **修改 lex_sql.l**：
   ```bash
   # 在 STRING_T 后面添加 CHAR
   STRING_T                                RETURN_TOKEN(STRING_T);
   CHAR                                    RETURN_TOKEN(STRING_T);
   ```

2. **重新生成解析器文件**：
   ```bash
   cd src/observer/sql/parser
   ./gen_parser.sh
   ```

3. **提交更新**：
   ```bash
   git add src/observer/sql/parser/lex_sql.l
   git add src/observer/sql/parser/yacc_sql.cpp src/observer/sql/parser/yacc_sql.hpp
   git add src/observer/sql/parser/lex_sql.cpp src/observer/sql/parser/lex_sql.h
   git commit -m "修复: 添加 CHAR 关键字支持，修复 CREATE TABLE 解析失败问题"
   git push
   ```

## 验证

修复后，以下 SQL 应该能正常解析：
- `CREATE TABLE t_basic(id int, age int, name char(4), score float);`
- `CREATE TABLE t(id int, name char(10));`

## 注意事项

1. **CHAR 关键字必须在 STRING_T 之后，ID_KEY 之前**
   - 这样 `char` 会优先匹配 `CHAR` 关键字，而不是 `ID_KEY`
   - Flex 的匹配规则：优先匹配前面更具体的规则

2. **确保解析器文件已更新**
   - 修改 `.l` 和 `.y` 文件后，必须运行 `gen_parser.sh` 重新生成
   - 生成的文件必须提交到 Git

3. **大小写不敏感**
   - 由于设置了 `%option case-insensitive`，`CHAR`、`char`、`Char` 都会被识别

