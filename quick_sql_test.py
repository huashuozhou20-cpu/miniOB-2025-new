#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
简化版 SQL 功能测试 - 使用已编译的二进制文件
"""

import socket
import time
import subprocess
import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OBSERVER_BIN = os.path.join(SCRIPT_DIR, "build/bin/observer")
TEST_DIR = "/tmp/miniob_final_test"
SOCKET_FILE = os.path.join(TEST_DIR, "miniob.sock")

def cleanup():
    import shutil
    if os.path.exists(TEST_DIR):
        shutil.rmtree(TEST_DIR)
    os.makedirs(TEST_DIR, exist_ok=True)

def start_observer():
    cleanup()
    cmd = [
        OBSERVER_BIN,
        "-s", SOCKET_FILE,
        "-f", os.path.join(SCRIPT_DIR, "etc/observer.ini"),
        "-P", "mysql",
        "-t", "mvcc",
        "-d", "disk"
    ]
    
    log_file = open(os.path.join(TEST_DIR, "observer.log"), "w")
    process = subprocess.Popen(cmd, stdout=log_file, stderr=log_file, cwd=TEST_DIR)
    
    for i in range(20):
        if os.path.exists(SOCKET_FILE):
            time.sleep(1)
            return process
        time.sleep(0.5)
    
    log_file.close()
    process.kill()
    process.wait()
    return None

def run_sql(sock_path, sql):
    try:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        sock.settimeout(5)
        sock.connect(sock_path)
        sock.sendall((sql + "\0").encode('utf-8'))
        
        response = b""
        while True:
            data = sock.recv(4096)
            if not data:
                break
            response += data
            if b'\x00' in data:
                break
        
        sock.close()
        result = response.rstrip(b'\x00').decode('utf-8', errors='ignore')
        return True, result
    except Exception as e:
        return False, str(e)

def main():
    print("=" * 70)
    print("MiniOB SQL 功能综合测试")
    print("=" * 70)
    
    if not os.path.exists(OBSERVER_BIN):
        print(f"ERROR: Observer binary not found at {OBSERVER_BIN}")
        return False
    
    print("\n启动 observer...")
    observer_proc = start_observer()
    if not observer_proc:
        print("ERROR: Observer failed to start")
        return False
    
    try:
        print("等待 observer 就绪...")
        time.sleep(2)
        
        test_cases = [
            # DDL
            ("CREATE TABLE - 基本类型", "CREATE TABLE t1(id int, name char, score float);", True),
            ("CREATE TABLE - VECTOR 类型", "CREATE TABLE t2(id int, vec vector(3));", True),
            ("CREATE TABLE - 大小写", "create table t3(id int);", True),
            
            # DML
            ("INSERT", "INSERT INTO t1 VALUES (1, 'a', 1.0), (2, 'b', 2.0);", True),
            ("SELECT *", "SELECT * FROM t1;", True),
            ("SELECT WHERE", "SELECT * FROM t1 WHERE id=1;", True),
            ("UPDATE", "UPDATE t1 SET score=10.0 WHERE id=1;", True),
            ("DELETE", "DELETE FROM t1 WHERE id=2;", True),
            
            # JOIN
            ("CREATE for JOIN", "CREATE TABLE j1(id int, name char); CREATE TABLE j2(id int, num int);", True),
            ("INSERT for JOIN", "INSERT INTO j1 VALUES (1,'a'), (2,'b'); INSERT INTO j2 VALUES (1,10), (2,20);", True),
            ("INNER JOIN", "SELECT * FROM j1 INNER JOIN j2 ON j1.id=j2.id;", True),
            
            # VECTOR
            ("INSERT VECTOR", "INSERT INTO t2 VALUES (1, '[1.0,2.0,3.0]');", True),
            ("SELECT VECTOR", "SELECT * FROM t2;", True),
            
            # DROP
            ("DROP TABLE", "DROP TABLE t1;", True),
        ]
        
        passed = 0
        failed = 0
        
        for name, sql, expect_success in test_cases:
            print(f"\n[{name}]")
            print(f"SQL: {sql[:80]}...")
            
            # 处理多个 SQL 语句
            sqls = sql.split(';')
            for single_sql in sqls:
                single_sql = single_sql.strip()
                if not single_sql:
                    continue
                single_sql += ';'
                
                success, result = run_sql(SOCKET_FILE, single_sql)
                
                if success == expect_success:
                    if "SUCCESS" in result.upper() or ("Failed" not in result and expect_success):
                        print(f"  ✓ PASS")
                        passed += 1
                    else:
                        print(f"  ✗ FAIL: {result[:100]}")
                        failed += 1
                else:
                    if not expect_success:
                        print(f"  ✓ PASS (Expected failure)")
                        passed += 1
                    else:
                        print(f"  ✗ FAIL: {result[:100]}")
                        failed += 1
        
        print("\n" + "=" * 70)
        print("测试总结")
        print("=" * 70)
        print(f"通过: {passed}")
        print(f"失败: {failed}")
        print(f"总计: {passed + failed}")
        
        return failed == 0
        
    finally:
        print("\n停止 observer...")
        observer_proc.terminate()
        try:
            observer_proc.wait(timeout=5)
        except:
            observer_proc.kill()
            observer_proc.wait()

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)

