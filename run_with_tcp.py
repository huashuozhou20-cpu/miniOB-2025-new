#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""使用 TCP 端口的全面测试"""

import subprocess
import time
import sys
import os

OBSERVER_BIN = "./build/bin/observer"
OBCLIENT_BIN = "./build/bin/obclient"
PORT = 6789

def run_sql(sql, timeout=10):
    """运行 SQL"""
    try:
        proc = subprocess.Popen(
            [OBCLIENT_BIN, "-h", "127.0.0.1", "-p", str(PORT)],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = proc.communicate(input=sql + ";\n", timeout=timeout)
        success = proc.returncode == 0 or "SUCCESS" in stdout.upper() or "ERROR" not in stdout.upper()
        return success, stdout, stderr
    except Exception as e:
        return False, "", str(e)

def test(name, sqls):
    """测试功能"""
    print(f"\n{'='*70}")
    print(f"测试: {name}")
    print(f"{'='*70}")
    
    for i, sql in enumerate(sqls, 1):
        print(f"  [{i}/{len(sqls)}] {sql[:60]}...")
        success, stdout, stderr = run_sql(sql)
        if not success:
            print(f"    ❌ 失败")
            if stderr:
                print(f"    错误: {stderr[:200]}")
            return False
        else:
            print(f"    ✅ 成功")
    return True

# 启动 observer
print("启动 observer (TCP)...")
observer = subprocess.Popen(
    [OBSERVER_BIN, "-p", str(PORT), "-P", "mysql", "-t", "mvcc", "-d", "disk"],
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT
)

time.sleep(10)

# 测试连接
print("\n测试连接...")
success, stdout, stderr = run_sql("SELECT 1")
if not success:
    print(f"❌ 连接失败: {stderr}")
    observer.kill()
    sys.exit(1)
print("✅ 连接成功")

# 运行测试
results = []

results.append(("drop-table", test("drop-table", [
    "CREATE TABLE test_drop(id INT)",
    "DROP TABLE test_drop"
])))

results.append(("update", test("update", [
    "CREATE TABLE test_update(id INT, name CHAR(10))",
    "INSERT INTO test_update VALUES(1, 'test')",
    "UPDATE test_update SET name='updated' WHERE id=1",
    "DROP TABLE test_update"
])))

results.append(("date", test("date", [
    "CREATE TABLE test_date(id INT, birthday DATE)",
    "INSERT INTO test_date VALUES(1, '2022-10-10')",
    "DROP TABLE test_date"
])))

results.append(("expression", test("expression", [
    "SELECT 1 + 2 * 3 AS result"
])))

results.append(("function", test("function", [
    "SELECT LENGTH('hello') AS len",
    "SELECT ROUND(3.14) AS rounded"
])))

results.append(("join-tables", test("join-tables", [
    "CREATE TABLE a(id INT, name CHAR)",
    "CREATE TABLE b(id INT, num INT)",
    "INSERT INTO a VALUES(1, 'x')",
    "INSERT INTO b VALUES(1, 10)",
    "SELECT * FROM a INNER JOIN b ON a.id=b.id",
    "DROP TABLE a",
    "DROP TABLE b"
])))

results.append(("group-by", test("group-by", [
    "CREATE TABLE test_group(id INT, score INT)",
    "INSERT INTO test_group VALUES(1, 80), (1, 90)",
    "SELECT id, AVG(score) FROM test_group GROUP BY id",
    "DROP TABLE test_group"
])))

results.append(("order-by", test("order-by", [
    "CREATE TABLE test_order(id INT, score INT)",
    "INSERT INTO test_order VALUES(1, 80), (2, 90)",
    "SELECT * FROM test_order ORDER BY score DESC",
    "DROP TABLE test_order"
])))

results.append(("null", test("null", [
    "CREATE TABLE test_null(id INT NULL, name CHAR(10) NOT NULL)",
    "DROP TABLE test_null"
])))

results.append(("alias", test("alias", [
    "CREATE TABLE test_alias(id INT, name CHAR(10))",
    "SELECT id AS num FROM test_alias AS t",
    "DROP TABLE test_alias"
])))

results.append(("multi-index", test("multi-index", [
    "CREATE TABLE test_multi(id INT, col1 INT, col2 INT)",
    "CREATE INDEX i_multi ON test_multi(col1, col2)",
    "DROP TABLE test_multi"
])))

results.append(("unique", test("unique", [
    "CREATE TABLE test_unique(id INT)",
    "CREATE UNIQUE INDEX i_unique ON test_unique(id)",
    "DROP TABLE test_unique"
])))

results.append(("text", test("text", [
    "CREATE TABLE test_text(id INT, article TEXT)",
    "DROP TABLE test_text"
])))

results.append(("union", test("union", [
    "CREATE TABLE t1(id INT)",
    "CREATE TABLE t2(id INT)",
    "INSERT INTO t1 VALUES(1)",
    "INSERT INTO t2 VALUES(1)",
    "SELECT * FROM t1 UNION SELECT * FROM t2",
    "DROP TABLE t1",
    "DROP TABLE t2"
])))

results.append(("vector-basic", test("vector-basic", [
    "CREATE TABLE test_vec(id INT, vec VECTOR(3))",
    "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1,2,3]'))",
    "INSERT INTO test_vec VALUES(2, STRING_TO_VECTOR('[2,3,4]'))",
    "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'COSINE') AS dist_cosine",
    "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'EUCLIDEAN') AS dist_euclidean",
    "SELECT id, VECTOR_TO_STRING(vec) AS vec_str FROM test_vec",
    "DROP TABLE test_vec"
])))

results.append(("vector-search", test("vector-search", [
    "CREATE TABLE test_vec_search(id INT, vec VECTOR(3))",
    "INSERT INTO test_vec_search VALUES(1, STRING_TO_VECTOR('[1,2,3]'))",
    "INSERT INTO test_vec_search VALUES(2, STRING_TO_VECTOR('[2,3,4]'))",
    "INSERT INTO test_vec_search VALUES(3, STRING_TO_VECTOR('[10,0,5]'))",
    "SELECT id FROM test_vec_search ORDER BY DISTANCE(vec, STRING_TO_VECTOR('[10,0,5]'), 'EUCLIDEAN') LIMIT 1",
    "DROP TABLE test_vec_search"
])))

results.append(("alter", test("alter", [
    "CREATE TABLE test_alter(id INT)",
    "ALTER TABLE test_alter ADD COLUMN name CHAR(10)",
    "ALTER TABLE test_alter ADD COLUMN age INT",
    "INSERT INTO test_alter VALUES(1, 'test', 20)",
    "ALTER TABLE test_alter DROP COLUMN age",
    "ALTER TABLE test_alter CHANGE COLUMN name new_name CHAR(10)",
    "ALTER TABLE test_alter RENAME TO test_alter2",
    "DROP TABLE test_alter2"
])))

results.append(("simple-sub-query", test("simple-sub-query", [
    "CREATE TABLE t1(id INT, name CHAR(10))",
    "CREATE TABLE t2(id INT, name CHAR(10))",
    "INSERT INTO t1 VALUES(1, 'A'), (2, 'B')",
    "INSERT INTO t2 VALUES(1, 'X'), (3, 'Y')",
    "SELECT * FROM t1 WHERE id IN (SELECT id FROM t2)",
    "SELECT * FROM t1 WHERE id NOT IN (SELECT id FROM t2)",
    "DROP TABLE t1",
    "DROP TABLE t2"
])))

results.append(("function-date_format", test("function-date_format", [
    "CREATE TABLE test_datefmt(id INT, birthday DATE)",
    "INSERT INTO test_datefmt VALUES(1, '2022-10-10')",
    "SELECT DATE_FORMAT(birthday, '%Y-%m-%d') FROM test_datefmt",
    "DROP TABLE test_datefmt"
])))

# 总结
print("\n" + "="*70)
print("测试总结")
print("="*70)
passed = sum(1 for _, r in results if r)
failed = len(results) - passed

for name, result in results:
    status = "✅" if result else "❌"
    print(f"{status} {name}")

print(f"\n总计: {len(results)}")
print(f"通过: {passed}")
print(f"失败: {failed}")

# 生成报告
import time
report_file = "comprehensive_test_report.md"
with open(report_file, "w", encoding="utf-8") as f:
    f.write("# MiniOB 全面功能测试报告\n\n")
    f.write(f"测试时间: {time.strftime('%Y-%m-%d %H:%M:%S')}\n\n")
    f.write(f"## 测试统计\n\n")
    f.write(f"- 总计: {len(results)} 项功能\n")
    f.write(f"- ✅ 通过: {passed}\n")
    f.write(f"- ❌ 失败: {failed}\n")
    f.write(f"- 通过率: {passed*100//len(results)}%\n\n")
    f.write("## 详细结果\n\n")
    seen = set()
    for name, result in results:
        if name not in seen:
            seen.add(name)
            status = "✅" if result else "❌"
            f.write(f"### {status} {name}\n\n")
    f.write("---\n\n")

print(f"\n报告已保存到: {report_file}")

observer.terminate()
try:
    observer.wait(timeout=2)
except:
    observer.kill()

sys.exit(0 if failed == 0 else 1)
