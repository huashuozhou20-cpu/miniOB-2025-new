#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MiniOB 全面功能测试 - 使用 TCP 端口
"""

import subprocess
import time
import sys
import os

PORT = 6789
OBSERVER_BIN = "./build/bin/observer"
OBCLIENT_BIN = "./build/bin/obclient"

def run_sql(sql):
    """运行 SQL"""
    try:
        cmd = f'echo "{sql};" | {OBCLIENT_BIN} -h 127.0.0.1 -p {PORT}'
        proc = subprocess.run(
            cmd,
            shell=True,
            capture_output=True,
            text=True,
            timeout=10
        )
        stdout = proc.stdout
        stderr = proc.stderr
        
        # 检查是否成功
        success = (
            proc.returncode == 0 and 
            "ERROR" not in stdout.upper() and
            "failed" not in stdout.lower() and
            "error" not in stderr.lower() and
            "Connection refused" not in stderr
        )
        return success, stdout, stderr
    except subprocess.TimeoutExpired:
        return False, "", "Timeout"
    except Exception as e:
        return False, "", str(e)

def test(name, sqls, description=""):
    """测试功能"""
    print(f"\n{'='*70}")
    print(f"测试: {name}")
    if description:
        print(f"描述: {description}")
    print(f"{'='*70}")
    
    for i, sql in enumerate(sqls, 1):
        print(f"  [{i}/{len(sqls)}] {sql[:60]}...")
        success, stdout, stderr = run_sql(sql)
        
        if not success:
            print(f"    ❌ 失败")
            if stderr:
                print(f"    错误: {stderr[:200]}")
            if stdout:
                print(f"    输出: {stdout[:200]}")
            return False
        else:
            print(f"    ✅ 成功")
            if stdout and len(stdout.strip()) > 0 and len(stdout) < 200:
                print(f"    输出: {stdout.strip()}")
    
    print(f"  ✅ {name} 测试通过")
    return True

def main():
    print("="*70)
    print("MiniOB 全面功能测试 (TCP 端口)")
    print("="*70)
    
    # 启动 observer
    print("\n启动 observer...")
    observer = subprocess.Popen(
        [OBSERVER_BIN, "-p", str(PORT), "-P", "mysql", "-t", "mvcc", "-d", "disk"],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT
    )
    
    # 等待 observer 启动
    print("等待 observer 启动...")
    for i in range(20):
        time.sleep(1)
        success, _, _ = run_sql("SELECT 1")
        if success:
            print(f"✅ Observer 启动成功 (等待了 {i+1} 秒)")
            break
        if i % 5 == 0:
            print(f"  等待中... ({i}秒)")
    else:
        print("❌ Observer 启动超时")
        observer.kill()
        sys.exit(1)
    
    # 再次测试连接确认
    print("\n测试连接...")
    success, stdout, stderr = run_sql("SELECT 1")
    if not success:
        print(f"❌ 连接失败")
        print(f"stdout: {stdout[:200]}")
        print(f"stderr: {stderr[:200]}")
        observer.kill()
        sys.exit(1)
    print("✅ 连接成功")
    
    results = []
    
    try:
        # ========== 基础功能测试 ==========
        
        results.append(("drop-table", test("drop-table", [
            "CREATE TABLE test_drop(id INT, name CHAR(10))",
            "INSERT INTO test_drop VALUES(1, 'test')",
            "DROP TABLE test_drop"
        ], "DROP TABLE 功能")))
        
        results.append(("update", test("update", [
            "CREATE TABLE test_update(id INT, name CHAR(10), score INT)",
            "INSERT INTO test_update VALUES(1, 'test1', 80)",
            "UPDATE test_update SET name='updated', score=90 WHERE id=1",
            "DROP TABLE test_update"
        ], "UPDATE 语句")))
        
        results.append(("date", test("date", [
            "CREATE TABLE test_date(id INT, birthday DATE)",
            "INSERT INTO test_date VALUES(1, '2022-10-10')",
            "INSERT INTO test_date VALUES(2, '2023-12-25')",
            "DROP TABLE test_date"
        ], "DATE 类型")))
        
        results.append(("expression", test("expression", [
            "CREATE TABLE test_expr(id INT, a INT, b INT)",
            "INSERT INTO test_expr VALUES(1, 10, 20)",
            "SELECT id, a + b AS sum, a * b AS product FROM test_expr",
            "SELECT * FROM test_expr WHERE a + b > 25",
            "DROP TABLE test_expr"
        ], "算术表达式")))
        
        results.append(("function", test("function", [
            "SELECT LENGTH('hello') AS len",
            "SELECT ROUND(3.14159) AS rounded",
            "CREATE TABLE test_func(id INT, name CHAR(10), score FLOAT, birthday DATE)",
            "INSERT INTO test_func VALUES(1, 'test', 85.5, '2022-10-10')",
            "SELECT LENGTH(name), ROUND(score), DATE_FORMAT(birthday, '%Y-%m-%d') FROM test_func",
            "DROP TABLE test_func"
        ], "系统函数: LENGTH, ROUND, DATE_FORMAT")))
        
        results.append(("join-tables", test("join-tables", [
            "CREATE TABLE a(id INT, name CHAR(10))",
            "CREATE TABLE b(id INT, num INT)",
            "INSERT INTO a VALUES(1, 'x'), (2, 'y')",
            "INSERT INTO b VALUES(1, 10), (2, 20)",
            "SELECT * FROM a INNER JOIN b ON a.id=b.id",
            "SELECT a.id, a.name, b.num FROM a, b WHERE a.id=b.id",
            "DROP TABLE a",
            "DROP TABLE b"
        ], "INNER JOIN")))
        
        results.append(("group-by", test("group-by", [
            "CREATE TABLE test_group(id INT, name CHAR(10), score INT)",
            "INSERT INTO test_group VALUES(1, 'A', 80), (1, 'B', 90), (2, 'C', 85)",
            "SELECT id, AVG(score) AS avg_score, MAX(score) AS max_score, COUNT(*) AS cnt FROM test_group GROUP BY id",
            "SELECT id, SUM(score) FROM test_group GROUP BY id HAVING SUM(score) > 80",
            "DROP TABLE test_group"
        ], "GROUP BY 和聚合函数")))
        
        results.append(("order-by", test("order-by", [
            "CREATE TABLE test_order(id INT, score INT, name CHAR(10))",
            "INSERT INTO test_order VALUES(1, 80, 'A'), (2, 90, 'B'), (3, 85, 'C')",
            "SELECT * FROM test_order ORDER BY score DESC",
            "SELECT * FROM test_order ORDER BY score ASC, name DESC",
            "DROP TABLE test_order"
        ], "ORDER BY 排序")))
        
        results.append(("null", test("null", [
            "CREATE TABLE test_null(id INT NULL, name CHAR(10) NOT NULL, score INT NULL)",
            "INSERT INTO test_null VALUES(1, 'test', NULL)",
            "INSERT INTO test_null VALUES(NULL, 'test2', 80)",
            "DROP TABLE test_null"
        ], "NULL 值处理")))
        
        results.append(("alias", test("alias", [
            "CREATE TABLE test_alias(id INT, name CHAR(10))",
            "INSERT INTO test_alias VALUES(1, 'test')",
            "SELECT id AS num, name AS n FROM test_alias AS t",
            "SELECT t.id, t.name FROM test_alias AS t",
            "DROP TABLE test_alias"
        ], "表别名和列别名")))
        
        results.append(("multi-index", test("multi-index", [
            "CREATE TABLE test_multi(id INT, col1 INT, col2 INT, col3 INT)",
            "CREATE INDEX i_multi ON test_multi(col1, col2)",
            "INSERT INTO test_multi VALUES(1, 10, 20, 30)",
            "DROP TABLE test_multi"
        ], "多字段索引")))
        
        results.append(("unique", test("unique", [
            "CREATE TABLE test_unique(id INT, name CHAR(10))",
            "CREATE UNIQUE INDEX i_unique ON test_unique(id)",
            "INSERT INTO test_unique VALUES(1, 'test1')",
            "INSERT INTO test_unique VALUES(2, 'test2')",
            "DROP TABLE test_unique"
        ], "唯一索引")))
        
        results.append(("text", test("text", [
            "CREATE TABLE test_text(id INT, article TEXT)",
            "INSERT INTO test_text VALUES(1, 'This is a long text article for testing')",
            "DROP TABLE test_text"
        ], "TEXT 类型")))
        
        # ========== 进阶功能测试 ==========
        
        results.append(("union", test("union", [
            "CREATE TABLE t1(id INT, name CHAR(10))",
            "CREATE TABLE t2(id INT, name CHAR(10))",
            "INSERT INTO t1 VALUES(1, 'A'), (2, 'B')",
            "INSERT INTO t2 VALUES(1, 'A'), (3, 'C')",
            "SELECT * FROM t1 UNION SELECT * FROM t2",
            "SELECT * FROM t1 UNION ALL SELECT * FROM t2",
            "DROP TABLE t1",
            "DROP TABLE t2"
        ], "UNION 和 UNION ALL")))
        
        results.append(("vector-basic", test("vector-basic", [
            "CREATE TABLE test_vec(id INT, vec VECTOR(3))",
            "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1,2,3]'))",
            "INSERT INTO test_vec VALUES(2, STRING_TO_VECTOR('[2,3,4]'))",
            "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'COSINE') AS dist_cosine",
            "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'EUCLIDEAN') AS dist_euclidean",
            "SELECT id, VECTOR_TO_STRING(vec) AS vec_str FROM test_vec",
            "DROP TABLE test_vec"
        ], "向量类型基础")))
        
        results.append(("vector-search", test("vector-search", [
            "CREATE TABLE test_vec_search(id INT, vec VECTOR(3))",
            "INSERT INTO test_vec_search VALUES(1, STRING_TO_VECTOR('[1,2,3]'))",
            "INSERT INTO test_vec_search VALUES(2, STRING_TO_VECTOR('[2,3,4]'))",
            "INSERT INTO test_vec_search VALUES(3, STRING_TO_VECTOR('[10,0,5]'))",
            "SELECT id FROM test_vec_search ORDER BY DISTANCE(vec, STRING_TO_VECTOR('[10,0,5]'), 'EUCLIDEAN') LIMIT 1",
            "DROP TABLE test_vec_search"
        ], "向量检索")))
        
        results.append(("alter", test("alter", [
            "CREATE TABLE test_alter(id INT)",
            "ALTER TABLE test_alter ADD COLUMN name CHAR(10)",
            "ALTER TABLE test_alter ADD COLUMN age INT",
            "INSERT INTO test_alter VALUES(1, 'test', 20)",
            "ALTER TABLE test_alter DROP COLUMN age",
            "ALTER TABLE test_alter CHANGE COLUMN name new_name CHAR(10)",
            "ALTER TABLE test_alter RENAME TO test_alter2",
            "DROP TABLE test_alter2"
        ], "ALTER TABLE")))
        
        results.append(("simple-sub-query", test("simple-sub-query", [
            "CREATE TABLE t1(id INT, name CHAR(10))",
            "CREATE TABLE t2(id INT, name CHAR(10))",
            "INSERT INTO t1 VALUES(1, 'A'), (2, 'B')",
            "INSERT INTO t2 VALUES(1, 'X'), (3, 'Y')",
            "SELECT * FROM t1 WHERE id IN (SELECT id FROM t2)",
            "SELECT * FROM t1 WHERE id NOT IN (SELECT id FROM t2)",
            "DROP TABLE t1",
            "DROP TABLE t2"
        ], "简单子查询")))
        
        # ========== 高级功能测试 ==========
        
        results.append(("create-view", test("create-view", [
            "CREATE TABLE view_t1(id INT, name CHAR(10))",
            "CREATE TABLE view_t2(id INT, age INT)",
            "INSERT INTO view_t1 VALUES(1, 'A'), (2, 'B')",
            "INSERT INTO view_t2 VALUES(1, 20), (2, 30)",
            "CREATE VIEW v1 AS SELECT t1.id, t1.name, t2.age FROM view_t1 t1, view_t2 t2 WHERE t1.id=t2.id",
            "SELECT * FROM v1",
            "DROP VIEW v1",
            "DROP TABLE view_t1",
            "DROP TABLE view_t2"
        ], "CREATE VIEW")))
        
        # 测试总结
        print("\n" + "="*70)
        print("测试总结")
        print("="*70)
        
        passed = sum(1 for _, r in results if r)
        failed = len(results) - passed
        
        print(f"\n总计: {len(results)} 项功能")
        print(f"✅ 通过: {passed}")
        print(f"❌ 失败: {failed}")
        
        print("\n详细结果:")
        for name, result in results:
            status = "✅" if result else "❌"
            print(f"  {status} {name}")
        
        # 生成报告
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
            for name, result in results:
                status = "✅" if result else "❌"
                f.write(f"### {status} {name}\n\n")
                f.write("---\n\n")
        
        print(f"\n报告已保存到: {report_file}")
        
        if failed == 0:
            print("\n🎉 所有测试通过！")
            return 0
        else:
            print(f"\n⚠️  {failed} 个测试失败")
            return 1
    
    finally:
        print("\n清理环境...")
        observer.terminate()
        try:
            observer.wait(timeout=3)
        except subprocess.TimeoutExpired:
            observer.kill()

if __name__ == "__main__":
    sys.exit(main())

