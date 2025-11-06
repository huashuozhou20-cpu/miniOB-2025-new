#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MiniOB 全部功能 SQL 测试脚本 (TCP 方式)
使用 TCP 连接测试所有功能
"""

import os
import sys
import subprocess
import tempfile
import time
import signal
import shutil

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
BUILD_DIR = os.path.join(SCRIPT_DIR, "build")
OBSERVER_BIN = os.path.join(BUILD_DIR, "bin", "observer")
OBCLIENT_BIN = os.path.join(BUILD_DIR, "bin", "obclient")

# 使用 TCP 方式
SERVER_HOST = "127.0.0.1"
SERVER_PORT = 6789

def check_binaries():
    """检查二进制文件是否存在"""
    if not os.path.exists(OBSERVER_BIN):
        print(f"错误: observer 不存在: {OBSERVER_BIN}")
        return False
    if not os.path.exists(OBCLIENT_BIN):
        print(f"错误: obclient 不存在: {OBCLIENT_BIN}")
        return False
    return True

def run_sql(host, port, sql, timeout=10):
    """运行 SQL 语句 (TCP 方式)"""
    try:
        cmd = [OBCLIENT_BIN, "-h", host, "-p", str(port)]
        proc = subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = proc.communicate(input=sql, timeout=timeout)
        # obclient 可能返回非零退出码但实际成功
        # 检查是否有错误信息
        has_error = "ERROR" in stderr.upper() or "FAIL" in stderr.upper() or "Connection refused" in stderr
        success = not has_error and proc.returncode == 0
        return success, stdout, stderr
    except subprocess.TimeoutExpired:
        proc.kill()
        return False, "", "Timeout"
    except Exception as e:
        return False, "", str(e)

def test_feature(name, host, port, test_steps):
    """测试一个功能"""
    print(f"\n{'='*70}")
    print(f"测试: {name}")
    print(f"{'='*70}")
    
    errors = []
    for i, (step_name, sql, expected_success) in enumerate(test_steps, 1):
        print(f"  [{i}/{len(test_steps)}] {step_name}")
        print(f"      SQL: {sql[:80]}...")
        
        success, stdout, stderr = run_sql(host, port, sql)
        
        if success == expected_success:
            print(f"      ✓ 通过")
            if stdout.strip():
                print(f"      输出: {stdout.strip()[:100]}")
        else:
            print(f"      ✗ 失败")
            if stderr:
                print(f"      错误: {stderr[:200]}")
            if stdout:
                print(f"      输出: {stdout[:200]}")
            errors.append(f"{step_name}: {stderr[:100] if stderr else stdout[:100]}")
    
    if errors:
        print(f"  ❌ {name} 测试失败 ({len(errors)}/{len(test_steps)} 步骤失败)")
        return False, errors
    else:
        print(f"  ✅ {name} 测试通过 ({len(test_steps)}/{len(test_steps)} 步骤成功)")
        return True, []

def main():
    print("="*70)
    print("MiniOB 全部功能 SQL 测试 (TCP 方式)")
    print("="*70)
    
    if not check_binaries():
        sys.exit(1)
    
    # 创建工作目录
    work_dir = tempfile.mkdtemp(prefix="miniob_test_")
    
    print(f"\n工作目录: {work_dir}")
    print(f"服务器: {SERVER_HOST}:{SERVER_PORT}")
    
    # 启动 observer (TCP)
    print("\n启动 observer (TCP)...")
    observer_log = os.path.join(work_dir, "observer.log")
    observer_proc = subprocess.Popen(
        [OBSERVER_BIN, "-p", str(SERVER_PORT), "-P", "mysql", "-t", "mvcc", "-d", "disk"],
        stdout=open(observer_log, "w"),
        stderr=subprocess.STDOUT
    )
    
    # 等待服务器启动
    print("等待 observer 启动...")
    time.sleep(5)  # 给 observer 足够的启动时间
    
    # 测试连接
    print("测试连接...")
    success, stdout, stderr = run_sql(SERVER_HOST, SERVER_PORT, "SELECT 1;")
    if not success:
        print("✗ 连接失败")
        print(f"错误: {stderr}")
        with open(observer_log, "r") as f:
            content = f.read()
            print("\nObserver 日志:")
            print(content[-1000:])
        observer_proc.kill()
        sys.exit(1)
    
    print("✓ Observer 启动成功并可以连接")
    
    # 测试结果
    results = {}
    
    try:
        # 1. drop-table
        results["drop-table"] = test_feature("drop-table", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_drop(id INT, name CHAR(10));", True),
            ("删除表", "DROP TABLE test_drop;", True),
        ])
        
        # 2. update
        results["update"] = test_feature("update", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_update(id INT, name CHAR(10), score INT);", True),
            ("插入数据", "INSERT INTO test_update VALUES(1, 'test', 80);", True),
            ("更新数据", "UPDATE test_update SET name='updated', score=90 WHERE id=1;", True),
            ("验证更新", "SELECT * FROM test_update WHERE id=1;", True),
            ("清理", "DROP TABLE test_update;", True),
        ])
        
        # 3. date
        results["date"] = test_feature("date", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_date(id INT, birthday DATE);", True),
            ("插入日期", "INSERT INTO test_date VALUES(1, '2022-10-10');", True),
            ("查询日期", "SELECT * FROM test_date;", True),
            ("清理", "DROP TABLE test_date;", True),
        ])
        
        # 4. expression
        results["expression"] = test_feature("expression", SERVER_HOST, SERVER_PORT, [
            ("算数表达式", "SELECT 1 + 2 * 3 AS result;", True),
            ("复杂表达式", "SELECT 10 - 5 / 2 AS result;", True),
        ])
        
        # 5. function
        results["function"] = test_feature("function", SERVER_HOST, SERVER_PORT, [
            ("LENGTH函数", "SELECT LENGTH('hello') AS len;", True),
            ("ROUND函数", "SELECT ROUND(3.14159) AS rounded;", True),
            ("DATE_FORMAT函数", "SELECT DATE_FORMAT('2022-10-10', '%Y-%m-%d') AS formatted;", True),
        ])
        
        # 6. join-tables
        results["join-tables"] = test_feature("join-tables", SERVER_HOST, SERVER_PORT, [
            ("创建表A", "CREATE TABLE a(id INT, name CHAR(10));", True),
            ("创建表B", "CREATE TABLE b(id INT, num INT);", True),
            ("插入数据A", "INSERT INTO a VALUES(1, 'x'), (2, 'y');", True),
            ("插入数据B", "INSERT INTO b VALUES(1, 10), (2, 20);", True),
            ("INNER JOIN", "SELECT * FROM a INNER JOIN b ON a.id=b.id;", True),
            ("多表JOIN", "SELECT a.id, a.name, b.num FROM a INNER JOIN b ON a.id=b.id WHERE a.id=1;", True),
            ("清理", "DROP TABLE a; DROP TABLE b;", True),
        ])
        
        # 7. group-by
        results["group-by"] = test_feature("group-by", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_group(id INT, score INT);", True),
            ("插入数据", "INSERT INTO test_group VALUES(1, 80), (1, 90), (2, 70);", True),
            ("GROUP BY", "SELECT id, AVG(score) FROM test_group GROUP BY id;", True),
            ("COUNT聚合", "SELECT id, COUNT(*) FROM test_group GROUP BY id;", True),
            ("清理", "DROP TABLE test_group;", True),
        ])
        
        # 8. order-by
        results["order-by"] = test_feature("order-by", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_order(id INT, score INT);", True),
            ("插入数据", "INSERT INTO test_order VALUES(1, 80), (2, 90), (3, 70);", True),
            ("ORDER BY ASC", "SELECT * FROM test_order ORDER BY score ASC;", True),
            ("ORDER BY DESC", "SELECT * FROM test_order ORDER BY score DESC;", True),
            ("多字段排序", "SELECT * FROM test_order ORDER BY score DESC, id ASC;", True),
            ("清理", "DROP TABLE test_order;", True),
        ])
        
        # 9. null
        results["null"] = test_feature("null", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_null(id INT NULL, name CHAR(10) NOT NULL);", True),
            ("插入NULL", "INSERT INTO test_null VALUES(NULL, 'test');", True),
            ("插入非NULL", "INSERT INTO test_null VALUES(1, 'test2');", True),
            ("查询", "SELECT * FROM test_null;", True),
            ("清理", "DROP TABLE test_null;", True),
        ])
        
        # 10. alias
        results["alias"] = test_feature("alias", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_alias(id INT, name CHAR(10));", True),
            ("列别名", "SELECT id AS num, name AS n FROM test_alias;", True),
            ("表别名", "SELECT t.id, t.name FROM test_alias AS t;", True),
            ("清理", "DROP TABLE test_alias;", True),
        ])
        
        # 11. multi-index
        results["multi-index"] = test_feature("multi-index", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_multi(id INT, col1 INT, col2 INT);", True),
            ("创建多字段索引", "CREATE INDEX i_multi ON test_multi(col1, col2);", True),
            ("插入数据", "INSERT INTO test_multi VALUES(1, 10, 20);", True),
            ("使用索引查询", "SELECT * FROM test_multi WHERE col1=10 AND col2=20;", True),
            ("清理", "DROP TABLE test_multi;", True),
        ])
        
        # 12. unique
        results["unique"] = test_feature("unique", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_unique(id INT);", True),
            ("创建唯一索引", "CREATE UNIQUE INDEX i_unique ON test_unique(id);", True),
            ("插入数据", "INSERT INTO test_unique VALUES(1);", True),
            ("插入重复数据", "INSERT INTO test_unique VALUES(1);", False),  # 应该失败
            ("清理", "DROP TABLE test_unique;", True),
        ])
        
        # 13. text
        results["text"] = test_feature("text", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_text(id INT, article TEXT);", True),
            ("插入文本", "INSERT INTO test_text VALUES(1, 'This is a long text article.');", True),
            ("查询文本", "SELECT * FROM test_text;", True),
            ("清理", "DROP TABLE test_text;", True),
        ])
        
        # 14. union
        results["union"] = test_feature("union", SERVER_HOST, SERVER_PORT, [
            ("创建表1", "CREATE TABLE t1(id INT);", True),
            ("创建表2", "CREATE TABLE t2(id INT);", True),
            ("插入数据1", "INSERT INTO t1 VALUES(1), (2);", True),
            ("插入数据2", "INSERT INTO t2 VALUES(2), (3);", True),
            ("UNION", "SELECT * FROM t1 UNION SELECT * FROM t2;", True),
            ("UNION ALL", "SELECT * FROM t1 UNION ALL SELECT * FROM t2;", True),
            ("清理", "DROP TABLE t1; DROP TABLE t2;", True),
        ])
        
        # 15. vector-basic
        results["vector-basic"] = test_feature("vector-basic", SERVER_HOST, SERVER_PORT, [
            ("创建向量表", "CREATE TABLE test_vec(id INT, vec VECTOR(3));", True),
            ("插入向量", "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1,2,3]'));", True),
            ("DISTANCE函数", "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'COSINE') AS dist;", True),
            ("EUCLIDEAN距离", "SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'EUCLIDEAN') AS dist;", True),
            ("VECTOR_TO_STRING", "SELECT VECTOR_TO_STRING(vec) FROM test_vec;", True),
            ("清理", "DROP TABLE test_vec;", True),
        ])
        
        # 16. alter
        results["alter"] = test_feature("alter", SERVER_HOST, SERVER_PORT, [
            ("创建表", "CREATE TABLE test_alter(id INT);", True),
            ("ADD COLUMN", "ALTER TABLE test_alter ADD COLUMN name CHAR(10);", True),
            ("查询新列", "SELECT * FROM test_alter;", True),
            ("DROP COLUMN", "ALTER TABLE test_alter DROP COLUMN name;", True),
            ("RENAME TABLE", "ALTER TABLE test_alter RENAME TO test_alter2;", True),
            ("清理", "DROP TABLE test_alter2;", True),
        ])
        
        # 17. simple-sub-query
        results["simple-sub-query"] = test_feature("simple-sub-query", SERVER_HOST, SERVER_PORT, [
            ("创建表1", "CREATE TABLE ssq1(id INT);", True),
            ("创建表2", "CREATE TABLE ssq2(id INT);", True),
            ("插入数据1", "INSERT INTO ssq1 VALUES(1), (2), (3);", True),
            ("插入数据2", "INSERT INTO ssq2 VALUES(2), (3);", True),
            ("IN子查询", "SELECT * FROM ssq1 WHERE id IN (SELECT id FROM ssq2);", True),
            ("清理", "DROP TABLE ssq1; DROP TABLE ssq2;", True),
        ])
        
        # 生成测试报告
        print("\n" + "="*70)
        print("测试总结")
        print("="*70)
        
        passed = sum(1 for success, _ in results.values() if success)
        failed = len(results) - passed
        
        print(f"\n通过: {passed}/{len(results)}")
        print(f"失败: {failed}/{len(results)}")
        
        print("\n详细结果:")
        for name, (success, errors) in sorted(results.items()):
            status = "✅" if success else "❌"
            print(f"  {status} {name}")
            if errors:
                for error in errors:
                    print(f"      - {error}")
        
        # 生成测试报告文件
        report_file = os.path.join(SCRIPT_DIR, "SQL_TEST_REPORT.md")
        with open(report_file, "w", encoding="utf-8") as f:
            f.write("# MiniOB SQL 功能测试报告\n\n")
            f.write(f"**测试时间**: {time.strftime('%Y-%m-%d %H:%M:%S')}\n\n")
            f.write(f"**测试结果**: {passed}/{len(results)} 通过\n\n")
            f.write("## 测试详情\n\n")
            
            for name, (success, errors) in sorted(results.items()):
                status = "✅" if success else "❌"
                f.write(f"### {status} {name}\n\n")
                if errors:
                    f.write("**错误信息**:\n")
                    for error in errors:
                        f.write(f"- {error}\n")
                    f.write("\n")
            
            f.write(f"\n## 总结\n\n")
            f.write(f"- 总测试项: {len(results)}\n")
            f.write(f"- 通过: {passed}\n")
            f.write(f"- 失败: {failed}\n")
            f.write(f"- 成功率: {passed*100//len(results) if len(results) > 0 else 0}%\n")
        
        print(f"\n测试报告已保存到: {report_file}")
        
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
        
        shutil.rmtree(work_dir, ignore_errors=True)

if __name__ == "__main__":
    sys.exit(main())

