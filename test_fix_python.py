#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# SQL修复验证测试脚本

import subprocess
import time
import os
import sys
import tempfile
import socket

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OBSERVER_BIN = os.path.join(SCRIPT_DIR, "build/bin/observer")
OBCLIENT_BIN = os.path.join(SCRIPT_DIR, "build/bin/obclient")

TEST_DIR = tempfile.mkdtemp(prefix="miniob_test_")
SOCKET_FILE = os.path.join(TEST_DIR, "miniob.sock")

def cleanup():
    """清理资源"""
    subprocess.run(["pkill", "-9", "observer"], stderr=subprocess.DEVNULL)
    import shutil
    if os.path.exists(TEST_DIR):
        shutil.rmtree(TEST_DIR)

def start_observer():
    """启动observer"""
    print("启动 observer...")
    log_file = os.path.join(TEST_DIR, "observer.log")
    proc = subprocess.Popen(
        [OBSERVER_BIN, "-s", SOCKET_FILE, "-P", "mysql", "-t", "mvcc", "-d", "disk"],
        stdout=open(log_file, "w"),
        stderr=subprocess.STDOUT
    )
    
    # 等待socket文件创建
    for i in range(20):
        if os.path.exists(SOCKET_FILE):
            time.sleep(2)
            print(f"✓ Observer 启动成功 (PID: {proc.pid})")
            return proc
        time.sleep(1)
    
    print("✗ Observer 启动失败")
    with open(log_file, "r") as f:
        print(f.read()[-500:])
    return None

def run_sql(sql):
    """执行SQL并返回结果"""
    try:
        proc = subprocess.Popen(
            [OBCLIENT_BIN, "-s", SOCKET_FILE],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        stdout, stderr = proc.communicate(input=sql + "\n", timeout=10)
        return proc.returncode, stdout, stderr
    except Exception as e:
        return -1, "", str(e)

def test_sql(name, sql, expected="SUCCESS"):
    """测试SQL语句"""
    print(f"\n{'='*60}")
    print(f"测试: {name}")
    print(f"SQL: {sql}")
    print("-"*60)
    
    exit_code, stdout, stderr = run_sql(sql)
    
    # 过滤输出
    output = stdout + stderr
    lines = [l for l in output.split('\n') if l.strip() and 'Welcome' not in l and 'Copyright' not in l and 'Learn' not in l]
    result = '\n'.join(lines)
    
    print(f"退出码: {exit_code}")
    if result:
        print(f"输出:\n{result}")
    
    # 判断结果
    has_error = any(keyword in result.lower() for keyword in ['error', 'fail', 'sql_syntax', 'failed to parse'])
    
    if has_error:
        if expected == "FAILURE":
            print("✓ PASS (期望失败，实际失败)")
            return True
        else:
            print("✗ FAIL (期望成功，但实际失败)")
            return False
    elif exit_code == 0 or exit_code == 4:
        if expected == "SUCCESS":
            print("✓ PASS (期望成功，实际成功)")
            return True
        else:
            print("✗ FAIL (期望失败，但实际成功)")
            return False
    else:
        print(f"? UNKNOWN (退出码: {exit_code})")
        return False

def test_query(name, sql, expected_values):
    """测试查询并检查结果"""
    print(f"\n{'='*60}")
    print(f"测试: {name}")
    print(f"SQL: {sql}")
    print("-"*60)
    
    exit_code, stdout, stderr = run_sql(sql)
    
    output = stdout + stderr
    lines = [l for l in output.split('\n') if l.strip() and 'Welcome' not in l and 'Copyright' not in l and 'Learn' not in l]
    result = '\n'.join(lines)
    
    print(f"退出码: {exit_code}")
    print(f"查询结果:\n{result}")
    print("-"*60)
    
    if exit_code != 0 and exit_code != 4:
        print("✗ FAIL (查询执行失败)")
        return False
    
    # 检查期望值
    all_found = True
    for value in expected_values:
        if value in result:
            print(f"✓ 找到期望值: {value}")
        else:
            print(f"✗ 未找到期望值: {value}")
            all_found = False
    
    if all_found:
        print("✓ PASS (所有期望值都找到)")
        return True
    else:
        print("✗ FAIL (部分期望值未找到)")
        return False

def main():
    print("="*60)
    print("MiniOB SQL 修复验证测试")
    print("使用报错日志中的SQL语句")
    print("="*60)
    
    if not os.path.exists(OBSERVER_BIN):
        print(f"错误: observer 不存在: {OBSERVER_BIN}")
        sys.exit(1)
    
    if not os.path.exists(OBCLIENT_BIN):
        print(f"错误: obclient 不存在: {OBCLIENT_BIN}")
        sys.exit(1)
    
    observer_proc = start_observer()
    if not observer_proc:
        cleanup()
        sys.exit(1)
    
    time.sleep(1)
    
    passed = 0
    failed = 0
    
    try:
        # 测试1: NULL/NOT NULL 语法 - create-view
        if test_sql("create-view", "create table create_view_t2(id int not null, age int, name char(10));", "SUCCESS"):
            passed += 1
        else:
            failed += 1
        
        # 测试2: NULL/NOT NULL 语法 - group-by
        if test_sql("group-by", "create table t_group_by (id int not null, score int not null, name char(1) null);", "SUCCESS"):
            passed += 1
        else:
            failed += 1
        
        # 测试3: NULL/NOT NULL 语法 - null
        if test_sql("null", "CREATE TABLE null_table(id int not null, num int null, price float not null, birthday date null);", "SUCCESS"):
            passed += 1
        else:
            failed += 1
        
        # 测试4: NULL/NOT NULL 语法 - order-by
        if test_sql("order-by", "create table t_order_by_3(id int null, score float);", "SUCCESS"):
            passed += 1
        else:
            failed += 1
        
        # 测试5: 浮点数精度 - expression
        print("\n准备测试数据...")
        run_sql("create table exp_table(id int, col1 int, col2 int, col3 float, col4 float);")
        run_sql("insert into exp_table VALUES (8, 8, 9, 8.01, 3.15);")
        run_sql("insert into exp_table VALUES (3, 8, 6, 5.12, 9.94);")
        run_sql("insert into exp_table VALUES (2, 3, 7, 8.48, 4.79);")
        
        if test_query("expression", 
                      "select id,-(col2*(-5)+6)+(col4+3)*(col1+col3*2),(9+col2)*col3*9 from exp_table where -(col2*(-6)+5)+(col4+4)*(col1+col3*4) > (2+col2)*col3*1;",
                      ["184.49", "260.03", "186.72"]):
            passed += 1
        else:
            failed += 1
        
    finally:
        observer_proc.terminate()
        time.sleep(1)
        observer_proc.kill()
        cleanup()
    
    print("\n" + "="*60)
    print("测试完成")
    print(f"通过: {passed}")
    print(f"失败: {failed}")
    print("="*60)
    
    sys.exit(0 if failed == 0 else 1)

if __name__ == "__main__":
    main()

