#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Simple test script for JOIN functionality
"""

import socket
import time
import subprocess
import os
import sys

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OBSERVER_BIN = os.path.join(SCRIPT_DIR, "build/bin/observer")
CLIENT_BIN = os.path.join(SCRIPT_DIR, "build/bin/obclient")
TEST_DIR = "/tmp/miniob_test_join"
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

def test_join():
    """Run JOIN tests"""
    print("Starting observer...")
    observer_proc = start_observer()
    if not observer_proc:
        return False
    
    try:
        print("Waiting for observer to be ready...")
        time.sleep(2)
        
        # Test 1: Create tables
        print("\n=== Creating tables ===")
        success, result = run_sql_unix_socket(SOCKET_FILE, "CREATE TABLE join_table_1(id int, name char);")
        print(f"CREATE TABLE 1: {'OK' if success else 'FAIL'}")
        if not success:
            print(f"Error: {result}")
        
        success, result = run_sql_unix_socket(SOCKET_FILE, "CREATE TABLE join_table_2(id int, num int);")
        print(f"CREATE TABLE 2: {'OK' if success else 'FAIL'}")
        
        success, result = run_sql_unix_socket(SOCKET_FILE, "CREATE TABLE join_table_3(id int, num2 int);")
        print(f"CREATE TABLE 3: {'OK' if success else 'FAIL'}")
        
        # Test 2: Insert data
        print("\n=== Inserting data ===")
        test_sqls = [
            "INSERT INTO join_table_1 VALUES (1, 'a');",
            "INSERT INTO join_table_1 VALUES (2, 'b');",
            "INSERT INTO join_table_1 VALUES (3, 'c');",
            "INSERT INTO join_table_2 VALUES (1, 2);",
            "INSERT INTO join_table_2 VALUES (2, 15);",
            "INSERT INTO join_table_3 VALUES (1, 120);",
            "INSERT INTO join_table_3 VALUES (3, 800);",
        ]
        
        for sql in test_sqls:
            success, result = run_sql_unix_socket(SOCKET_FILE, sql)
            if not success:
                print(f"INSERT failed: {result}")
        
        # Test 3: Simple JOIN
        print("\n=== Test 1: Simple INNER JOIN ===")
        success, result = run_sql_unix_socket(SOCKET_FILE, 
            "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id;")
        print(f"Result: {'SUCCESS' if success else 'FAIL'}")
        if success:
            print(result)
        else:
            print(f"Error: {result}")
        
        # Test 4: Three-table JOIN
        print("\n=== Test 2: Three-table JOIN ===")
        success, result = run_sql_unix_socket(SOCKET_FILE,
            "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id INNER JOIN join_table_3 ON join_table_1.id=join_table_3.id;")
        print(f"Result: {'SUCCESS' if success else 'FAIL'}")
        if success:
            print(result)
        else:
            print(f"Error: {result}")
        
        # Test 5: JOIN with WHERE
        print("\n=== Test 3: JOIN with WHERE clause ===")
        success, result = run_sql_unix_socket(SOCKET_FILE,
            "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id AND join_table_2.num>13 WHERE join_table_1.name='b';")
        print(f"Result: {'SUCCESS' if success else 'FAIL'}")
        if success:
            print(result)
        else:
            print(f"Error: {result}")
        
        return True
        
    finally:
        print("\nStopping observer...")
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
    
    success = test_join()
    sys.exit(0 if success else 1)

