#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
综合功能测试脚本 - 检查主要功能是否可用
"""

import socket
import time
import subprocess
import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OBSERVER_BIN = os.path.join(SCRIPT_DIR, "build/bin/observer")
TEST_DIR = "/tmp/miniob_function_test"
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
    process = subprocess.Popen(cmd, stdout=log_file, stderr=log_file)
    
    # Wait for socket file to appear
    max_wait = 10
    for i in range(max_wait):
        if os.path.exists(SOCKET_FILE):
            time.sleep(1)  # Give it a moment to fully initialize
            return process
        time.sleep(0.5)
    
    log_file.close()
    process.kill()
    process.wait()
    print("ERROR: Observer failed to start")
    with open(os.path.join(TEST_DIR, "observer.log"), "r") as f:
        print(f.read())
    return None

def run_sql_unix_socket(sock_path, sql):
    """Run SQL via unix socket"""
    try:
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
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

def test_case(name, sql, expected_success=True):
    """Run a test case"""
    print(f"\n[{name}]")
    print(f"SQL: {sql}")
    success, result = run_sql_unix_socket(SOCKET_FILE, sql)
    if success == expected_success:
        status = "✓ PASS"
        if result and "SUCCESS" in result.upper():
            print(f"Status: {status}")
            if result.strip() and "SUCCESS" not in result.upper():
                print(f"Result: {result.strip()[:100]}")
        elif expected_success:
            print(f"Status: {status}")
            if result.strip():
                print(f"Result: {result.strip()[:200]}")
        else:
            print(f"Status: {status}")
    else:
        status = "✗ FAIL"
        print(f"Status: {status}")
        print(f"Error: {result}")
    return success == expected_success

def run_comprehensive_tests():
    """Run comprehensive functionality tests"""
    print("=" * 60)
    print("MiniOB 功能综合测试")
    print("=" * 60)
    
    print("\n启动 observer...")
    observer_proc = start_observer()
    if not observer_proc:
        return False
    
    try:
        print("等待 observer 就绪...")
        time.sleep(2)
        
        results = []
        
        # Test 1: CREATE TABLE
        print("\n" + "=" * 60)
        print("测试 1: CREATE TABLE")
        print("=" * 60)
        results.append(test_case("CREATE TABLE 1", 
            "CREATE TABLE test_table_1(id int, name char, score float);", True))
        results.append(test_case("CREATE TABLE 2", 
            "CREATE TABLE test_table_2(id int, num int);", True))
        results.append(test_case("CREATE TABLE 3", 
            "CREATE TABLE test_table_3(id int, num2 int);", True))
        
        # Test 2: INSERT
        print("\n" + "=" * 60)
        print("测试 2: INSERT")
        print("=" * 60)
        results.append(test_case("INSERT single", 
            "INSERT INTO test_table_1 VALUES (1, 'a', 1.0);", True))
        results.append(test_case("INSERT multiple", 
            "INSERT INTO test_table_1 VALUES (2, 'b', 2.0), (3, 'c', 3.0);", True))
        results.append(test_case("INSERT into table 2", 
            "INSERT INTO test_table_2 VALUES (1, 10), (2, 20);", True))
        results.append(test_case("INSERT into table 3", 
            "INSERT INTO test_table_3 VALUES (1, 100), (3, 300);", True))
        
        # Test 3: SELECT
        print("\n" + "=" * 60)
        print("测试 3: SELECT")
        print("=" * 60)
        results.append(test_case("SELECT *", 
            "SELECT * FROM test_table_1;", True))
        results.append(test_case("SELECT WHERE", 
            "SELECT * FROM test_table_1 WHERE id=1;", True))
        results.append(test_case("SELECT columns", 
            "SELECT id, name FROM test_table_1;", True))
        
        # Test 4: UPDATE
        print("\n" + "=" * 60)
        print("测试 4: UPDATE")
        print("=" * 60)
        results.append(test_case("UPDATE", 
            "UPDATE test_table_1 SET score=10.5 WHERE id=1;", True))
        results.append(test_case("SELECT after UPDATE", 
            "SELECT * FROM test_table_1 WHERE id=1;", True))
        
        # Test 5: DELETE
        print("\n" + "=" * 60)
        print("测试 5: DELETE")
        print("=" * 60)
        results.append(test_case("DELETE", 
            "DELETE FROM test_table_1 WHERE id=2;", True))
        results.append(test_case("SELECT after DELETE", 
            "SELECT * FROM test_table_1;", True))
        
        # Test 6: JOIN
        print("\n" + "=" * 60)
        print("测试 6: JOIN (多表连接)")
        print("=" * 60)
        results.append(test_case("INNER JOIN", 
            "SELECT * FROM test_table_1 INNER JOIN test_table_2 ON test_table_1.id=test_table_2.id;", True))
        results.append(test_case("Three-table JOIN", 
            "SELECT * FROM test_table_1 INNER JOIN test_table_2 ON test_table_1.id=test_table_2.id INNER JOIN test_table_3 ON test_table_1.id=test_table_3.id;", True))
        results.append(test_case("JOIN with WHERE", 
            "SELECT * FROM test_table_1 INNER JOIN test_table_2 ON test_table_1.id=test_table_2.id WHERE test_table_1.score>5;", True))
        
        # Test 7: DROP TABLE
        print("\n" + "=" * 60)
        print("测试 7: DROP TABLE")
        print("=" * 60)
        results.append(test_case("DROP TABLE", 
            "DROP TABLE test_table_1;", True))
        results.append(test_case("DROP TABLE (verify)", 
            "SELECT * FROM test_table_1;", False))  # Should fail
        
        # Summary
        print("\n" + "=" * 60)
        print("测试总结")
        print("=" * 60)
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
        observer_proc.terminate()
        try:
            observer_proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            observer_proc.kill()
            observer_proc.wait()

if __name__ == "__main__":
    if not os.path.exists(OBSERVER_BIN):
        print(f"ERROR: Observer binary not found at {OBSERVER_BIN}")
        sys.exit(1)
    
    success = run_comprehensive_tests()
    sys.exit(0 if success else 1)

