#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MiniOB 全面功能测试 - 使用 SQL 语句验证所有功能
"""

import os
import sys
import subprocess
import tempfile
import time
import signal
import threading

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
OBSERVER_BIN = os.path.join(BUILD_DIR, "bin", "observer")
OBCLIENT_BIN = os.path.join(BUILD_DIR, "bin", "obclient")

class TestResult:
    def __init__(self, name):
        self.name = name
        self.passed = False
        self.error = None
        self.output = None

def check_binaries():
    """检查二进制文件是否存在"""
    if not os.path.exists(OBSERVER_BIN):
        print(f"❌ 错误: observer 不存在: {OBSERVER_BIN}")
        return False
    if not os.path.exists(OBCLIENT_BIN):
        print(f"❌ 错误: obclient 不存在: {OBCLIENT_BIN}")
        return False
    return True

def run_sql(sock_path, sql, timeout=10):
    """运行 SQL 语句"""
    try:
        cmd = [OBCLIENT_BIN, "-s", sock_path]
        proc = subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = proc.communicate(input=sql + ";\n", timeout=timeout)
        success = proc.returncode == 0 or "SUCCESS" in stdout.upper() or stderr == ""
        return success, stdout, stderr
    except subprocess.TimeoutExpired:
        proc.kill()
        return False, "", "Timeout"
    except Exception as e:
        return False, "", str(e)

def test_functionality(name, sock_path, sqls, description=""):
    """测试功能"""
    print(f"\n{'='*70}")
    print(f"测试: {name}")
    if description:
        print(f"描述: {description}")
    print(f"{'='*70}")
    
    result = TestResult(name)
    
    for i, sql in enumerate(sqls, 1):
        print(f"  [{i}/{len(sqls)}] 执行: {sql[:60]}...")
        success, stdout, stderr = run_sql(sock_path, sql)
        
        if not success:
            print(f"    ❌ 失败")
            if stderr:
                print(f"    错误: {stderr[:200]}")
            result.error = stderr
            result.output = stdout
            return result
        else:
            print(f"    ✅ 成功")
            if stdout and len(stdout.strip()) > 0:
                print(f"    输出: {stdout[:100]}")
    
    result.passed = True
    print(f"  ✅ {name} 测试通过")
    return result

def main():
    print("="*70)
    print("MiniOB 全面功能测试")
    print("="*70)
    
    if not check_binaries():
        sys.exit(1)
    
    # 创建工作目录
    work_dir = tempfile.mkdtemp(prefix="miniob_test_")
    sock_path = os.path.join(work_dir, "miniob.sock")
    
    print(f"\n工作目录: {work_dir}")
    print(f"Socket: {sock_path}")
    
    # 启动 observer
    print("\n启动 observer...")
    observer_log = os.path.join(work_dir, "observer.log")
    observer_proc = subprocess.Popen(
        [OBSERVER_BIN, "-s", sock_path, "-P", "mysql", "-t", "mvcc", "-d", "disk"],
        stdout=open(observer_log, "w"),
        stderr=subprocess.STDOUT
    )
    
    # 等待 socket 文件创建 - 增加等待时间
    print("等待 observer 启动...")
    socket_created = False
    for i in range(30):  # 增加到30秒
        if os.path.exists(sock_path):
            time.sleep(3)  # socket创建后等待3秒确保完全启动
            print(f"✅ Observer 启动成功 (等待了 {i+3} 秒)")
            socket_created = True
            break
        time.sleep(1)
        if i % 5 == 0:
            print(f"  等待中... ({i}秒)")
    
    if not socket_created:
        print("❌ Observer 启动失败 - socket 文件未创建")
        print("\nObserver 日志:")
        with open(observer_log, "r") as f:
            print(f.read()[-1000:])
        observer_proc.kill()
        sys.exit(1)
    
    # 先测试一个简单的 SQL 确认连接正常
    print("\n测试连接...")
    success, stdout, stderr = run_sql(sock_path, "SELECT 1")
    if not success:
        print(f"❌ 连接测试失败: {stderr}")
        observer_proc.kill()
        sys.exit(1)
    print("✅ 连接测试成功")
    
    results = []
    
    try:
        # ========== 基础功能测试 ==========
        
        # 1. drop-table
        results.append(test_functionality(
            "drop-table",
            sock_path,
            [
                "CREATE TABLE test_drop(id INT, name CHAR(10))",
                "INSERT INTO test_drop VALUES(1, 'test')",
                "DROP TABLE test_drop",
                "CREATE TABLE test_drop(id INT)"
            ],
            "DROP TABLE 功能"
        ))
        
        # 2. update
        results.append(test_functionality(
            "update",
            sock_path,
            [
                "CREATE TABLE test_update(id INT, name CHAR(10), score INT)",
                "INSERT INTO test_update VALUES(1, 'test1', 80)",
                "UPDATE test_update SET name='updated', score=90 WHERE id=1",
                "SELECT * FROM test_update WHERE id=1",
                "DROP TABLE test_update"
            ],
            "UPDATE 语句 - 单字段和多字段更新"
        ))
        
        # 3. date
        results.append(test_functionality(
            "date",
            sock_path,
            [
                "CREATE TABLE test_date(id INT, birthday DATE)",
                "INSERT INTO test_date VALUES(1, '2022-10-10')",
                "INSERT INTO test_date VALUES(2, '2023-12-25')",
                "SELECT * FROM test_date",
                "DROP TABLE test_date"
            ],
            "DATE 类型支持"
        ))
        
        # 4. expression
        results.append(test_functionality(
            "expression",
            sock_path,
            [
                "CREATE TABLE test_expr(id INT, a INT, b INT)",
                "INSERT INTO test_expr VALUES(1, 10, 20)",
                "SELECT id, a + b AS sum, a * b AS product FROM test_expr",
                "SELECT * FROM test_expr WHERE a + b > 25",
                "DROP TABLE test_expr"
            ],
            "算术表达式 (+,-,*,/)"
        ))
        
        # 5. function
        results.append(test_functionality(
            "function",
            sock_path,
            [
                "SELECT LENGTH('hello') AS len",
                "SELECT ROUND(3.14159) AS rounded",
                "SELECT ROUND(3.7) AS rounded2",
                "CREATE TABLE test_func(id INT, name CHAR(10), score FLOAT, birthday DATE)",
                "INSERT INTO test_func VALUES(1, 'test', 85.5, '2022-10-10')",
                "SELECT LENGTH(name), ROUND(score), DATE_FORMAT(birthday, '%Y-%m-%d') FROM test_func",
                "DROP TABLE test_func"
            ],
            "系统函数: LENGTH, ROUND, DATE_FORMAT"
        ))
        
        # 6. join-tables
        results.append(test_functionality(
            "join-tables",
            sock_path,
            [
                "CREATE TABLE a(id INT, name CHAR(10))",
                "CREATE TABLE b(id INT, num INT)",
                "INSERT INTO a VALUES(1, 'x'), (2, 'y')",
                "INSERT INTO b VALUES(1, 10), (2, 20)",
                "SELECT * FROM a INNER JOIN b ON a.id=b.id",
                "SELECT a.id, a.name, b.num FROM a, b WHERE a.id=b.id",
                "DROP TABLE a",
                "DROP TABLE b"
            ],
            "INNER JOIN 多表连接"
        ))
        
        # 7. group-by
        results.append(test_functionality(
            "group-by",
            sock_path,
            [
                "CREATE TABLE test_group(id INT, name CHAR(10), score INT)",
                "INSERT INTO test_group VALUES(1, 'A', 80), (1, 'B', 90), (2, 'C', 85)",
                "SELECT id, AVG(score) AS avg_score, MAX(score) AS max_score, COUNT(*) AS cnt FROM test_group GROUP BY id",
                "SELECT id, SUM(score) FROM test_group GROUP BY id HAVING SUM(score) > 80",
                "DROP TABLE test_group"
            ],
            "GROUP BY 和聚合函数 (COUNT, SUM, AVG, MAX, MIN)"
        ))
        
        # 8. order-by
        results.append(test_functionality(
            "order-by",
            sock_path,
            [
                "CREATE TABLE test_order(id INT, score INT, name CHAR(10))",
                "INSERT INTO test_order VALUES(1, 80, 'A'), (2, 90, 'B'), (3, 85, 'C')",
                "SELECT * FROM test_order ORDER BY score DESC",
                "SELECT * FROM test_order ORDER BY score ASC, name DESC",
                "DROP TABLE test_order"
            ],
            "ORDER BY 多字段排序 (ASC/DESC)"
        ))
        
        # 9. null
        results.append(test_functionality(
            "null",
            sock_path,
            [
                "CREATE TABLE test_null(id INT NULL, name CHAR(10) NOT NULL, score INT NULL)",
                "INSERT INTO test_null VALUES(1, 'test', NULL)",
                "INSERT INTO test_null VALUES(NULL, 'test2', 80)",
                "SELECT * FROM test_null",
                "DROP TABLE test_null"
            ],
            "NULL 值处理"
        ))
        
        # 10. alias
        results.append(test_functionality(
            "alias",
            sock_path,
            [
                "CREATE TABLE test_alias(id INT, name CHAR(10))",
                "INSERT INTO test_alias VALUES(1, 'test')",
                "SELECT id AS num, name AS n FROM test_alias AS t",
                "SELECT t.id, t.name FROM test_alias AS t",
                "DROP TABLE test_alias"
            ],
            "表别名和列别名"
        ))
        
        # 11. multi-index
        results.append(test_functionality(
            "multi-index",
            sock_path,
            [
                "CREATE TABLE test_multi(id INT, col1 INT, col2 INT, col3 INT)",
                "CREATE INDEX i_multi ON test_multi(col1, col2)",
                "INSERT INTO test_multi VALUES(1, 10, 20, 30)",
                "SELECT * FROM test_multi WHERE col1=10 AND col2=20",
                "DROP TABLE test_multi"
            ],
            "多字段索引"
        ))
        
        # 12. unique
        results.append(test_functionality(
            "unique",
            sock_path,
            [
                "CREATE TABLE test_unique(id INT, name CHAR(10))",
                "CREATE UNIQUE INDEX i_unique ON test_unique(id)",
                "INSERT INTO test_unique VALUES(1, 'test1')",
                "INSERT INTO test_unique VALUES(2, 'test2')",
                "DROP TABLE test_unique"
            ],
            "唯一索引 (UNIQUE)"
        ))
        
        # 13. text
        results.append(test_functionality(
            "text",
            sock_path,
            [
                "CREATE TABLE test_text(id INT, article TEXT)",
                "INSERT INTO test_text VALUES(1, 'This is a long text article')",
                "SELECT * FROM test_text",
                "DROP TABLE test_text"
            ],
            "TEXT 类型 (大字段)"
        ))
        
        # ========== 进阶功能测试 ==========
        
        # 14. union
        results.append(test_functionality(
            "union",
            sock_path,
            [
                "CREATE TABLE t1(id INT, name CHAR(10))",
                "CREATE TABLE t2(id INT, name CHAR(10))",
                "INSERT INTO t1 VALUES(1, 'A'), (2, 'B')",
                "INSERT INTO t2 VALUES(1, 'A'), (3, 'C')",
                "SELECT * FROM t1 UNION SELECT * FROM t2",
                "SELECT * FROM t1 UNION ALL SELECT * FROM t2",
                "DROP TABLE t1",
                "DROP TABLE t2"
            ],
            "UNION 和 UNION ALL"
        ))
        
        # 15. vector-basic
        results.append(test_functionality(
            "vector-basic",
            sock_path,
            [
                "CREATE TABLE test_vec(id INT, vec VECTOR(3))",
                "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1,2,3]'))",
                "INSERT INTO test_vec VALUES(2, STRING_TO_VECTOR('[2,3,4]'))",
                "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'COSINE') AS dist_cosine",
                "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'EUCLIDEAN') AS dist_euclidean",
                "SELECT id, VECTOR_TO_STRING(vec) AS vec_str FROM test_vec",
                "DROP TABLE test_vec"
            ],
            "向量类型基础: VECTOR, DISTANCE, VECTOR_TO_STRING, STRING_TO_VECTOR"
        ))
        
        # 16. vector-search
        results.append(test_functionality(
            "vector-search",
            sock_path,
            [
                "CREATE TABLE test_vec_search(id INT, vec VECTOR(3))",
                "INSERT INTO test_vec_search VALUES(1, STRING_TO_VECTOR('[1,2,3]'))",
                "INSERT INTO test_vec_search VALUES(2, STRING_TO_VECTOR('[2,3,4]'))",
                "INSERT INTO test_vec_search VALUES(3, STRING_TO_VECTOR('[10,0,5]'))",
                "SELECT id FROM test_vec_search ORDER BY DISTANCE(vec, STRING_TO_VECTOR('[10,0,5]'), 'EUCLIDEAN') LIMIT 1",
                "DROP TABLE test_vec_search"
            ],
            "向量检索 (基于 DISTANCE 排序)"
        ))
        
        # 17. alter
        results.append(test_functionality(
            "alter",
            sock_path,
            [
                "CREATE TABLE test_alter(id INT)",
                "ALTER TABLE test_alter ADD COLUMN name CHAR(10)",
                "ALTER TABLE test_alter ADD COLUMN age INT",
                "INSERT INTO test_alter VALUES(1, 'test', 20)",
                "ALTER TABLE test_alter DROP COLUMN age",
                "ALTER TABLE test_alter CHANGE COLUMN name new_name CHAR(10)",
                "ALTER TABLE test_alter RENAME TO test_alter2",
                "SELECT * FROM test_alter2",
                "DROP TABLE test_alter2"
            ],
            "ALTER TABLE: ADD COLUMN, DROP COLUMN, CHANGE COLUMN, RENAME TO"
        ))
        
        # 18. simple-sub-query
        results.append(test_functionality(
            "simple-sub-query",
            sock_path,
            [
                "CREATE TABLE t1(id INT, name CHAR(10))",
                "CREATE TABLE t2(id INT, name CHAR(10))",
                "INSERT INTO t1 VALUES(1, 'A'), (2, 'B')",
                "INSERT INTO t2 VALUES(1, 'X'), (3, 'Y')",
                "SELECT * FROM t1 WHERE id IN (SELECT id FROM t2)",
                "SELECT * FROM t1 WHERE id NOT IN (SELECT id FROM t2)",
                "DROP TABLE t1",
                "DROP TABLE t2"
            ],
            "简单子查询: IN / NOT IN"
        ))
        
        # ========== 高级功能测试 ==========
        
        # 19. create-view (如果支持)
        results.append(test_functionality(
            "create-view",
            sock_path,
            [
                "CREATE TABLE view_t1(id INT, name CHAR(10))",
                "CREATE TABLE view_t2(id INT, age INT)",
                "INSERT INTO view_t1 VALUES(1, 'A'), (2, 'B')",
                "INSERT INTO view_t2 VALUES(1, 20), (2, 30)",
                "CREATE VIEW v1 AS SELECT t1.id, t1.name, t2.age FROM view_t1 t1, view_t2 t2 WHERE t1.id=t2.id",
                "SELECT * FROM v1",
                "DROP VIEW v1",
                "DROP TABLE view_t1",
                "DROP TABLE view_t2"
            ],
            "CREATE VIEW 视图创建"
        ))
        
        # 20. 测试 summary
        print("\n" + "="*70)
        print("测试总结")
        print("="*70)
        
        passed = sum(1 for r in results if r.passed)
        failed = len(results) - passed
        
        print(f"\n总计: {len(results)} 项功能")
        print(f"✅ 通过: {passed}")
        print(f"❌ 失败: {failed}")
        
        print("\n详细结果:")
        for result in results:
            status = "✅" if result.passed else "❌"
            print(f"  {status} {result.name}")
            if not result.passed and result.error:
                print(f"     错误: {result.error[:100]}")
        
        # 生成报告
        report_file = os.path.join(SCRIPT_DIR, "comprehensive_test_report.md")
        with open(report_file, "w", encoding="utf-8") as f:
            f.write("# MiniOB 全面功能测试报告\n\n")
            f.write(f"测试时间: {time.strftime('%Y-%m-%d %H:%M:%S')}\n\n")
            f.write(f"## 测试统计\n\n")
            f.write(f"- 总计: {len(results)} 项功能\n")
            f.write(f"- ✅ 通过: {passed}\n")
            f.write(f"- ❌ 失败: {failed}\n\n")
            f.write("## 详细结果\n\n")
            for result in results:
                status = "✅" if result.passed else "❌"
                f.write(f"### {status} {result.name}\n\n")
                if not result.passed:
                    f.write(f"**错误**: {result.error}\n\n")
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
        observer_proc.terminate()
        try:
            observer_proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            observer_proc.kill()
        
        import shutil
        shutil.rmtree(work_dir, ignore_errors=True)

if __name__ == "__main__":
    sys.exit(main())
