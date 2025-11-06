#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 测试浮点数精度 - 简化版

import subprocess
import time
import os
import sys
import tempfile
import re

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OBSERVER_BIN = os.path.join(SCRIPT_DIR, "build/bin/observer")
OBCLIENT_BIN = os.path.join(SCRIPT_DIR, "build/bin/obclient")

TEST_DIR = tempfile.mkdtemp(prefix="miniob_test_")
SOCKET_FILE = os.path.join(TEST_DIR, "miniob.sock")

def cleanup():
    subprocess.run(["pkill", "-9", "observer"], stderr=subprocess.DEVNULL)
    import shutil
    if os.path.exists(TEST_DIR):
        shutil.rmtree(TEST_DIR)

def start_observer():
    print("启动 observer...")
    log_file = os.path.join(TEST_DIR, "observer.log")
    proc = subprocess.Popen(
        [OBSERVER_BIN, "-s", SOCKET_FILE, "-P", "mysql", "-t", "mvcc", "-d", "disk"],
        stdout=open(log_file, "w"),
        stderr=subprocess.STDOUT
    )
    
    for i in range(20):
        if os.path.exists(SOCKET_FILE):
            time.sleep(2)
            print(f"✓ Observer 启动成功")
            return proc
        time.sleep(1)
    return None

def run_sql(sql):
    proc = subprocess.Popen(
        [OBCLIENT_BIN, "-s", SOCKET_FILE],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = proc.communicate(input=sql + "\n", timeout=10)
    return proc.returncode, stdout, stderr

def extract_result(output):
    """提取查询结果"""
    lines = []
    for line in output.split('\n'):
        line = line.strip()
        if not line:
            continue
        if 'Welcome' in line or 'Copyright' in line or 'Learn' in line or 'miniob >' in line:
            continue
        # 提取数据行（包含数字的行）
        if re.match(r'^[\d\s\|\.\-]+$', line) or '|' in line:
            lines.append(line)
    return lines

def main():
    print("="*60)
    print("MiniOB SQL 修复验证测试")
    print("="*60)
    
    observer_proc = start_observer()
    if not observer_proc:
        cleanup()
        sys.exit(1)
    
    time.sleep(1)
    
    passed = 0
    failed = 0
    
    try:
        # 测试1-4: NULL/NOT NULL 语法
        print("\n【测试1-4】NULL/NOT NULL 语法测试")
        test_cases = [
            ("create-view", "create table create_view_t2(id int not null, age int, name char(10));"),
            ("group-by", "create table t_group_by (id int not null, score int not null, name char(1) null);"),
            ("null", "CREATE TABLE null_table(id int not null, num int null, price float not null, birthday date null);"),
            ("order-by", "create table t_order_by_3(id int null, score float);"),
        ]
        
        for name, sql in test_cases:
            exit_code, stdout, stderr = run_sql(sql)
            if exit_code == 0 or exit_code == 4:
                print(f"  ✓ {name}: PASS")
                passed += 1
            else:
                print(f"  ✗ {name}: FAIL (退出码: {exit_code})")
                failed += 1
        
        # 测试5: 浮点数精度
        print("\n【测试5】浮点数精度测试")
        print("  准备数据...")
        run_sql("create table exp_table(id int, col1 int, col2 int, col3 float, col4 float);")
        run_sql("insert into exp_table VALUES (8, 8, 9, 8.01, 3.15);")
        run_sql("insert into exp_table VALUES (3, 8, 6, 5.12, 9.94);")
        run_sql("insert into exp_table VALUES (2, 3, 7, 8.48, 4.79);")
        
        # 先查询所有数据
        print("  查询所有数据:")
        exit_code, stdout, stderr = run_sql("select * from exp_table;")
        result_lines = extract_result(stdout + stderr)
        for line in result_lines:
            print(f"    {line}")
        
        # 测试表达式计算（简化查询）
        print("\n  测试表达式计算 (id=2):")
        exit_code, stdout, stderr = run_sql("select -(col2*(-5)+6)+(col4+3)*(col1+col3*2) as result from exp_table where id=2;")
        result_lines = extract_result(stdout + stderr)
        print(f"    退出码: {exit_code}")
        print(f"    结果行数: {len(result_lines)}")
        for line in result_lines:
            print(f"    {line}")
            # 检查是否包含184.49或类似的浮点数
            if re.search(r'184\.4[89]|184\.49', line):
                print("    ✓ 找到期望值 184.49")
                passed += 1
                break
        else:
            print("    ✗ 未找到期望值 184.49")
            failed += 1
        
        # 测试完整查询
        print("\n  测试完整查询:")
        exit_code, stdout, stderr = run_sql("select id,-(col2*(-5)+6)+(col4+3)*(col1+col3*2),(9+col2)*col3*9 from exp_table where -(col2*(-6)+5)+(col4+4)*(col1+col3*4) > (2+col2)*col3*1;")
        result_lines = extract_result(stdout + stderr)
        print(f"    退出码: {exit_code}")
        print(f"    结果行数: {len(result_lines)}")
        for line in result_lines:
            print(f"    {line}")
        
        # 检查是否包含期望的值
        result_text = '\n'.join(result_lines)
        expected_found = 0
        for expected in ["184.49", "260.03", "186.72"]:
            if expected in result_text or re.search(expected.replace('.', r'\.'), result_text):
                print(f"    ✓ 找到期望值: {expected}")
                expected_found += 1
            else:
                print(f"    ✗ 未找到期望值: {expected}")
        
        if expected_found == 3:
            print("    ✓ 所有期望值都找到")
            passed += 1
        else:
            print(f"    ✗ 只找到 {expected_found}/3 个期望值")
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

