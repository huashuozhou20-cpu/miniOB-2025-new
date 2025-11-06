#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
使用已编译的二进制文件直接运行测试
"""

import os
import sys
import subprocess
import tempfile
import time
import signal

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
OBSERVER_BIN = os.path.join(BUILD_DIR, "bin", "observer")
OBCLIENT_BIN = os.path.join(BUILD_DIR, "bin", "obclient")

def check_binaries():
    """检查二进制文件是否存在"""
    if not os.path.exists(OBSERVER_BIN):
        print(f"错误: observer 不存在: {OBSERVER_BIN}")
        return False
    if not os.path.exists(OBCLIENT_BIN):
        print(f"错误: obclient 不存在: {OBCLIENT_BIN}")
        return False
    return True

def run_sql(sock_path, sql, timeout=5):
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
        stdout, stderr = proc.communicate(input=sql, timeout=timeout)
        return proc.returncode == 0, stdout, stderr
    except subprocess.TimeoutExpired:
        proc.kill()
        return False, "", "Timeout"
    except Exception as e:
        return False, "", str(e)

def test_case(name, sock_path, sqls, expected_success=True):
    """测试用例"""
    print(f"\n测试: {name}")
    all_success = True
    
    for i, sql in enumerate(sqls, 1):
        print(f"  [{i}/{len(sqls)}] {sql[:60]}...")
        success, stdout, stderr = run_sql(sock_path, sql)
        
        if not success:
            print(f"    ✗ 失败")
            print(f"    stderr: {stderr[:200]}")
            all_success = False
        else:
            print(f"    ✓ 成功")
    
    return all_success == expected_success

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
    
    # 等待 socket 文件创建
    print("等待 observer 启动...")
    for i in range(15):
        if os.path.exists(sock_path):
            time.sleep(2)  # 再等待一下确保完全启动
            print("✓ Observer 启动成功")
            break
        time.sleep(1)
    else:
        print("✗ Observer 启动失败")
        with open(observer_log, "r") as f:
            print(f.read()[-500:])
        observer_proc.kill()
        sys.exit(1)
    
    try:
        # 测试用例
        results = []
        
        # 基础功能
        print("\n" + "="*70)
        print("1. 基础功能测试")
        print("="*70)
        
        results.append(("drop-table", test_case("drop-table", sock_path, [
            "CREATE TABLE test_drop(id INT);",
            "DROP TABLE test_drop;"
        ])))
        
        results.append(("update", test_case("update", sock_path, [
            "CREATE TABLE test_update(id INT, name CHAR(10));",
            "INSERT INTO test_update VALUES(1, 'test');",
            "UPDATE test_update SET name='updated' WHERE id=1;",
            "DROP TABLE test_update;"
        ])))
        
        results.append(("date", test_case("date", sock_path, [
            "CREATE TABLE test_date(id INT, birthday DATE);",
            "INSERT INTO test_date VALUES(1, '2022-10-10');",
            "DROP TABLE test_date;"
        ])))
        
        results.append(("expression", test_case("expression", sock_path, [
            "SELECT 1 + 2 * 3 AS result;"
        ])))
        
        results.append(("function", test_case("function", sock_path, [
            "SELECT LENGTH('hello') AS len;",
            "SELECT ROUND(3.14) AS rounded;"
        ])))
        
        results.append(("join-tables", test_case("join-tables", sock_path, [
            "CREATE TABLE a(id INT, name CHAR);",
            "CREATE TABLE b(id INT, num INT);",
            "INSERT INTO a VALUES(1, 'x');",
            "INSERT INTO b VALUES(1, 10);",
            "SELECT * FROM a INNER JOIN b ON a.id=b.id;",
            "DROP TABLE a;",
            "DROP TABLE b;"
        ])))
        
        results.append(("group-by", test_case("group-by", sock_path, [
            "CREATE TABLE test_group(id INT, score INT);",
            "INSERT INTO test_group VALUES(1, 80), (1, 90);",
            "SELECT id, AVG(score) FROM test_group GROUP BY id;",
            "DROP TABLE test_group;"
        ])))
        
        results.append(("order-by", test_case("order-by", sock_path, [
            "CREATE TABLE test_order(id INT, score INT);",
            "INSERT INTO test_order VALUES(1, 80), (2, 90);",
            "SELECT * FROM test_order ORDER BY score DESC;",
            "DROP TABLE test_order;"
        ])))
        
        results.append(("null", test_case("null", sock_path, [
            "CREATE TABLE test_null(id INT NULL, name CHAR(10) NOT NULL);",
            "DROP TABLE test_null;"
        ])))
        
        results.append(("alias", test_case("alias", sock_path, [
            "CREATE TABLE test_alias(id INT);",
            "SELECT id AS num FROM test_alias AS t;",
            "DROP TABLE test_alias;"
        ])))
        
        results.append(("multi-index", test_case("multi-index", sock_path, [
            "CREATE TABLE test_multi(id INT, col1 INT, col2 INT);",
            "CREATE INDEX i_multi ON test_multi(col1, col2);",
            "DROP TABLE test_multi;"
        ])))
        
        results.append(("unique", test_case("unique", sock_path, [
            "CREATE TABLE test_unique(id INT);",
            "CREATE UNIQUE INDEX i_unique ON test_unique(id);",
            "DROP TABLE test_unique;"
        ])))
        
        results.append(("text", test_case("text", sock_path, [
            "CREATE TABLE test_text(id INT, article TEXT);",
            "DROP TABLE test_text;"
        ])))
        
        results.append(("union", test_case("union", sock_path, [
            "CREATE TABLE t1(id INT);",
            "CREATE TABLE t2(id INT);",
            "INSERT INTO t1 VALUES(1);",
            "INSERT INTO t2 VALUES(1);",
            "SELECT * FROM t1 UNION SELECT * FROM t2;",
            "DROP TABLE t1;",
            "DROP TABLE t2;"
        ])))
        
        results.append(("vector-basic", test_case("vector-basic", sock_path, [
            "CREATE TABLE test_vec(id INT, vec VECTOR(3));",
            "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1,2,3]'));",
            "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'COSINE') AS dist;",
            "DROP TABLE test_vec;"
        ])))
        
        results.append(("alter", test_case("alter", sock_path, [
            "CREATE TABLE test_alter(id INT);",
            "ALTER TABLE test_alter ADD COLUMN name CHAR(10);",
            "ALTER TABLE test_alter DROP COLUMN name;",
            "ALTER TABLE test_alter RENAME TO test_alter2;",
            "DROP TABLE test_alter2;"
        ])))
        
        # 总结
        print("\n" + "="*70)
        print("测试总结")
        print("="*70)
        
        passed = sum(1 for _, result in results if result)
        failed = len(results) - passed
        
        for name, result in results:
            status = "✓" if result else "✗"
            print(f"{status} {name}")
        
        print(f"\n通过: {passed}")
        print(f"失败: {failed}")
        print(f"总计: {len(results)}")
        
        if failed == 0:
            print("\n✓ 所有测试通过！")
            return 0
        else:
            print(f"\n✗ {failed} 个测试失败")
            return 1
    
    finally:
        print("\n清理环境...")
        observer_proc.terminate()
        observer_proc.wait(timeout=5)
        if observer_proc.poll() is None:
            observer_proc.kill()
        
        import shutil
        shutil.rmtree(work_dir, ignore_errors=True)

if __name__ == "__main__":
    sys.exit(main())

