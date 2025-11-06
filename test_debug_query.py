#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 调试查询问题

import subprocess
import time
import os
import sys
import tempfile

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
    return proc.returncode, stdout + stderr

def main():
    observer_proc = start_observer()
    if not observer_proc:
        cleanup()
        sys.exit(1)
    
    time.sleep(1)
    
    try:
        print("1. 创建表...")
        result = run_sql("create table exp_table(id int, col1 int, col2 int, col3 float, col4 float);")
        print(f"   结果: {result[0]}")
        
        print("\n2. 插入数据...")
        for values in ["(8, 8, 9, 8.01, 3.15)", "(3, 8, 6, 5.12, 9.94)", "(2, 3, 7, 8.48, 4.79)"]:
            result = run_sql(f"insert into exp_table VALUES {values};")
            print(f"   插入 {values}: {result[0]}")
        
        print("\n3. 简单查询所有数据...")
        result = run_sql("select * from exp_table;")
        print("   完整输出:")
        print(result[1][:2000])
        
        print("\n4. 计算表达式值...")
        # 手动计算 id=2 的值: -(7*(-5)+6)+(4.79+3)*(3+8.48*2) = -(-35+6)+7.79*(3+16.96) = -(-29)+7.79*19.96 = 29+155.4884 = 184.4884
        # 四舍五入到2位小数应该是 184.49
        result = run_sql("select id, col1, col2, col3, col4 from exp_table where id=2;")
        print("   数据:")
        print(result[1][:500])
        
        result = run_sql("select -(col2*(-5)+6)+(col4+3)*(col1+col3*2) as result from exp_table where id=2;")
        print("   表达式结果:")
        print(result[1][:500])
        
    finally:
        observer_proc.terminate()
        time.sleep(1)
        observer_proc.kill()
        cleanup()

if __name__ == "__main__":
    main()

