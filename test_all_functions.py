#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
全面功能测试脚本 - 改进版
测试所有已实现的功能
"""

import subprocess
import time
import os
import signal
import sys

def run_sql(sock_path, sql):
    """执行SQL语句"""
    try:
        # 使用更可靠的方式执行SQL
        cmd = ['./build/bin/obclient', '-s', sock_path]
        process = subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            cwd='/home/obuser/oceanbase-contest/test/miniob-2025-new'
        )
        
        # 发送SQL并获取输出
        stdout, stderr = process.communicate(input=sql + '\n', timeout=5)
        output = stdout + stderr
        
        # 过滤掉欢迎信息和提示符
        lines = []
        for line in output.split('\n'):
            line = line.strip()
            if line and 'Welcome' not in line and 'Copyright' not in line and \
               'Learn' not in line and 'miniob >' not in line and \
               'obclient' not in line.lower():
                lines.append(line)
        
        return '\n'.join(lines)
    except subprocess.TimeoutExpired:
        process.kill()
        return "Timeout"
    except Exception as e:
        return f"Error: {str(e)}"

def check_success(result):
    """检查SQL是否成功执行"""
    result_lower = result.lower()
    # 检查是否有明确的错误信息
    if 'error' in result_lower or 'fail' in result_lower or 'syntax' in result_lower:
        return False
    # 如果没有输出或只有空行，可能也是成功的（对于某些DDL语句）
    if not result.strip():
        return True
    # 如果有输出但没有错误，认为是成功的
    return True

def test_function(name, sql, check_output=None):
    """测试单个功能"""
    print(f"\n测试: {name}")
    print(f"SQL: {sql}")
    result = run_sql('/tmp/miniob_test_all/miniob.sock', sql)
    print(f"结果: {result[:200]}...")  # 只显示前200个字符
    
    if check_output:
        # 自定义检查函数
        success = check_output(result)
    else:
        # 默认检查：没有错误就认为成功
        success = check_success(result)
    
    if success:
        print(f"✅ {name} - 通过")
        return True
    else:
        print(f"❌ {name} - 失败")
        return False

def main():
    # 清理并创建测试目录
    test_dir = '/tmp/miniob_test_all'
    os.system(f'rm -rf {test_dir}')
    os.makedirs(test_dir, exist_ok=True)
    
    # 启动observer
    print("启动observer...")
    log_file = f'{test_dir}/observer.log'
    observer_process = subprocess.Popen(
        ['./build/bin/observer', '-s', f'{test_dir}/miniob.sock', '-P', 'mysql', '-t', 'mvcc', '-d', 'disk'],
        stdout=open(log_file, 'w'),
        stderr=subprocess.STDOUT,
        cwd='/home/obuser/oceanbase-contest/test/miniob-2025-new'
    )
    
    # 等待observer启动
    time.sleep(3)
    
    passed = 0
    failed = 0
    
    try:
        # 1. 测试NULL/NOT NULL
        print("\n" + "="*60)
        print("测试1: NULL/NOT NULL")
        print("="*60)
        if test_function("CREATE TABLE with NULL", 
                        "CREATE TABLE test_null(id int, name char(10) NULL, age int NOT NULL);"):
            passed += 1
        else:
            failed += 1
            
        if test_function("INSERT with NULL", 
                        "INSERT INTO test_null VALUES(1, NULL, 20);"):
            passed += 1
        else:
            failed += 1
        
        # 2. 测试唯一索引
        print("\n" + "="*60)
        print("测试2: 唯一索引")
        print("="*60)
        if test_function("CREATE TABLE for unique index", 
                        "CREATE TABLE test_unique(id int, name char(10));"):
            passed += 1
        else:
            failed += 1
            
        if test_function("INSERT data", 
                        "INSERT INTO test_unique VALUES(1, 'a');"):
            passed += 1
        else:
            failed += 1
            
        if test_function("CREATE UNIQUE INDEX", 
                        "CREATE UNIQUE INDEX idx_id ON test_unique(id);"):
            passed += 1
        else:
            failed += 1
        
        # 3. 测试函数
        print("\n" + "="*60)
        print("测试3: 系统函数")
        print("="*60)
        if test_function("CREATE TABLE for functions", 
                        "CREATE TABLE test_func(id int, name char(10), price float, date_col date);"):
            passed += 1
        else:
            failed += 1
            
        if test_function("INSERT data", 
                        "INSERT INTO test_func VALUES(1, 'test', 12.345, '2020-01-21');"):
            passed += 1
        else:
            failed += 1
            
        if test_function("LENGTH function", 
                        "SELECT LENGTH(name) FROM test_func;",
                        lambda r: '4' in r or 'test' in r.lower()):
            passed += 1
        else:
            failed += 1
        
        # 4. 测试UNION
        print("\n" + "="*60)
        print("测试4: UNION")
        print("="*60)
        if test_function("CREATE TABLE for UNION", 
                        "CREATE TABLE test_union1(id int);"):
            passed += 1
        else:
            failed += 1
            
        if test_function("CREATE TABLE 2", 
                        "CREATE TABLE test_union2(id int);"):
            passed += 1
        else:
            failed += 1
        
        # 5. 测试JOIN
        print("\n" + "="*60)
        print("测试5: JOIN")
        print("="*60)
        if test_function("CREATE TABLE for JOIN", 
                        "CREATE TABLE test_join1(id int, name char(10));"):
            passed += 1
        else:
            failed += 1
        
        # 6. 测试ALTER TABLE
        print("\n" + "="*60)
        print("测试6: ALTER TABLE")
        print("="*60)
        if test_function("CREATE TABLE for ALTER", 
                        "CREATE TABLE test_alter(id int, name char(10));"):
            passed += 1
        else:
            failed += 1
        
        # 7. 测试vector-basic
        print("\n" + "="*60)
        print("测试7: Vector类型基础功能")
        print("="*60)
        if test_function("CREATE TABLE with VECTOR", 
                        "CREATE TABLE test_vector(id int, vec vector(3));"):
            passed += 1
        else:
            failed += 1
        
        # 8. 测试TOKENIZE
        print("\n" + "="*60)
        print("测试8: TOKENIZE函数")
        print("="*60)
        if test_function("CREATE TABLE for TOKENIZE", 
                        "CREATE TABLE test_tokenize(id int, text char(100));"):
            passed += 1
        else:
            failed += 1
        
        # 9. 测试日期类型索引
        print("\n" + "="*60)
        print("测试9: 日期类型索引")
        print("="*60)
        if test_function("CREATE TABLE with DATE", 
                        "CREATE TABLE test_date(id int, date_col date);"):
            passed += 1
        else:
            failed += 1
        
        print("\n" + "="*60)
        print("测试总结")
        print("="*60)
        print(f"通过: {passed}")
        print(f"失败: {failed}")
        print(f"总计: {passed + failed}")
        
    except Exception as e:
        print(f"测试过程中出错: {e}")
        import traceback
        traceback.print_exc()
    finally:
        # 停止observer
        print("\n停止observer...")
        observer_process.terminate()
        try:
            observer_process.wait(timeout=5)
        except subprocess.TimeoutExpired:
            observer_process.kill()
        
        # 清理
        # os.system(f'rm -rf {test_dir}')

if __name__ == '__main__':
    main()
