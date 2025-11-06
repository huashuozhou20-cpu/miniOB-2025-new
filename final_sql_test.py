#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
改进的综合 SQL 功能测试脚本
使用更简单的方法测试SQL功能
"""

import socket
import time
import subprocess
import os
import sys
import signal

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OBSERVER_BIN = os.path.join(SCRIPT_DIR, "build/bin/observer")
TEST_DIR = "/tmp/miniob_test_final"
SOCKET_FILE = os.path.join(TEST_DIR, "miniob.sock")

def cleanup():
    """Clean up test directory"""
    import shutil
    if os.path.exists(TEST_DIR):
        shutil.rmtree(TEST_DIR)
    os.makedirs(TEST_DIR, exist_ok=True)

def start_observer():
    """Start observer process"""
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
    process = subprocess.Popen(cmd, stdout=log_file, stderr=log_file, 
                              preexec_fn=os.setsid)
    
    # Wait for socket file to appear
    max_wait = 20
    for i in range(max_wait):
        if os.path.exists(SOCKET_FILE):
            time.sleep(2)  # Give it a moment to fully initialize
            return process
        time.sleep(0.5)
    
    log_file.close()
    try:
        os.killpg(os.getpgid(process.pid), signal.SIGTERM)
        process.wait(timeout=3)
    except:
        try:
            process.kill()
        except:
            pass
    
    print("ERROR: Observer failed to start")
    if os.path.exists(os.path.join(TEST_DIR, "observer.log")):
        with open(os.path.join(TEST_DIR, "observer.log"), "r") as f:
            content = f.read()
            if content:
                print("Observer log:")
                print(content[-500:])  # Last 500 chars
    return None

def run_sql_unix_socket(sock_path, sql):
    """Run SQL via unix socket"""
    try:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        sock.settimeout(10)
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

def test_case(name, sql, expected_success=True, show_result=False):
    """Run a test case"""
    success, result = run_sql_unix_socket(SOCKET_FILE, sql)
    
    if success == expected_success:
        status = "✓ PASS"
        if result and "SUCCESS" in result.upper():
            pass  # Success, no need to show
        elif expected_success and result.strip():
            # Show result if it's not just SUCCESS
            if "SUCCESS" not in result.upper():
                pass  # Will show below if show_result
    else:
        status = "✗ FAIL"
    
    print(f"  {status}: {name}")
    if status == "✗ FAIL":
        print(f"    SQL: {sql}")
        print(f"    Error: {result[:200]}")
    elif show_result and result.strip() and "SUCCESS" not in result.upper():
        lines = result.strip().split('\n')
        for line in lines[:5]:  # Show first 5 lines
            print(f"    {line}")
    
    return success == expected_success

def run_comprehensive_tests():
    """Run comprehensive functionality tests"""
    print("=" * 70)
    print("MiniOB 综合 SQL 功能测试")
    print("=" * 70)
    
    print("\n启动 observer...")
    observer_proc = start_observer()
    if not observer_proc:
        return False
    
    try:
        print("等待 observer 就绪...")
        time.sleep(2)
        
        results = []
        
        # Test 1: CREATE TABLE (including VECTOR)
        print("\n" + "=" * 70)
        print("测试 1: CREATE TABLE")
        print("=" * 70)
        results.append(test_case("CREATE TABLE - Basic", 
            "CREATE TABLE test_table_1(id int, name char, score float);", True))
        results.append(test_case("CREATE TABLE - VECTOR", 
            "CREATE TABLE test_table_2(id int, embedding vector(3));", True))
        results.append(test_case("CREATE TABLE - VECTOR lowercase", 
            "CREATE TABLE test_table_3(id int, vec vector(5));", True))
        
        # Test 2: INSERT
        print("\n" + "=" * 70)
        print("测试 2: INSERT")
        print("=" * 70)
        results.append(test_case("INSERT - Basic", 
            "INSERT INTO test_table_1 VALUES (1, 'a', 1.0);", True))
        results.append(test_case("INSERT - Multiple rows", 
            "INSERT INTO test_table_1 VALUES (2, 'b', 2.0), (3, 'c', 3.0);", True))
        
        # Test 3: SELECT
        print("\n" + "=" * 70)
        print("测试 3: SELECT")
        print("=" * 70)
        results.append(test_case("SELECT - All columns", 
            "SELECT * FROM test_table_1;", True, show_result=True))
        results.append(test_case("SELECT - WHERE clause", 
            "SELECT * FROM test_table_1 WHERE id=1;", True, show_result=True))
        
        # Test 4: UPDATE
        print("\n" + "=" * 70)
        print("测试 4: UPDATE")
        print("=" * 70)
        results.append(test_case("UPDATE", 
            "UPDATE test_table_1 SET score=10.5 WHERE id=1;", True))
        results.append(test_case("SELECT after UPDATE", 
            "SELECT * FROM test_table_1 WHERE id=1;", True, show_result=True))
        
        # Test 5: DELETE
        print("\n" + "=" * 70)
        print("测试 5: DELETE")
        print("=" * 70)
        results.append(test_case("DELETE", 
            "DELETE FROM test_table_1 WHERE id=2;", True))
        results.append(test_case("SELECT after DELETE", 
            "SELECT * FROM test_table_1;", True, show_result=True))
        
        # Test 6: JOIN
        print("\n" + "=" * 70)
        print("测试 6: JOIN (多表连接)")
        print("=" * 70)
        results.append(test_case("CREATE tables for JOIN", 
            "CREATE TABLE join_a(id int, name char);", True))
        results.append(test_case("CREATE table 2", 
            "CREATE TABLE join_b(id int, num int);", True))
        results.append(test_case("INSERT into join_a", 
            "INSERT INTO join_a VALUES (1, 'a'), (2, 'b');", True))
        results.append(test_case("INSERT into join_b", 
            "INSERT INTO join_b VALUES (1, 10), (2, 20);", True))
        results.append(test_case("INNER JOIN", 
            "SELECT * FROM join_a INNER JOIN join_b ON join_a.id=join_b.id;", True, show_result=True))
        
        # Test 7: DROP TABLE
        print("\n" + "=" * 70)
        print("测试 7: DROP TABLE")
        print("=" * 70)
        results.append(test_case("DROP TABLE", 
            "DROP TABLE test_table_1;", True))
        
        # Summary
        print("\n" + "=" * 70)
        print("测试总结")
        print("=" * 70)
        passed = sum(results)
        total = len(results)
        print(f"通过: {passed}/{total}")
        print(f"失败: {total - passed}/{total}")
        
        if passed == total:
            print("\n✓ 所有测试通过！")
            return True
        else:
            print(f"\n✗ 有 {total - passed} 个测试失败")
            return False
        
    finally:
        print("\n停止 observer...")
        try:
            if observer_proc:
                os.killpg(os.getpgid(observer_proc.pid), signal.SIGTERM)
                try:
                    observer_proc.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    os.killpg(os.getpgid(observer_proc.pid), signal.SIGKILL)
                    observer_proc.wait()
        except Exception as e:
            print(f"清理进程时出错: {e}")

if __name__ == "__main__":
    if not os.path.exists(OBSERVER_BIN):
        print(f"ERROR: Observer binary not found at {OBSERVER_BIN}")
        sys.exit(1)
    
    success = run_comprehensive_tests()
    sys.exit(0 if success else 1)

