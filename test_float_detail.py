#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 详细测试浮点数精度

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

def main():
    observer_proc = start_observer()
    if not observer_proc:
        cleanup()
        sys.exit(1)
    
    time.sleep(1)
    
    try:
        print("\n1. 创建表...")
        exit_code, stdout, stderr = run_sql("create table exp_table(id int, col1 int, col2 int, col3 float, col4 float);")
        print(f"   退出码: {exit_code}")
        
        print("\n2. 插入数据...")
        for values in [
            "(8, 8, 9, 8.01, 3.15)",
            "(3, 8, 6, 5.12, 9.94)",
            "(2, 3, 7, 8.48, 4.79)"
        ]:
            exit_code, stdout, stderr = run_sql(f"insert into exp_table VALUES {values};")
            print(f"   插入 {values}: 退出码={exit_code}")
        
        print("\n3. 查询所有数据...")
        exit_code, stdout, stderr = run_sql("select * from exp_table;")
        print("   输出:")
        for line in (stdout + stderr).split('\n'):
            if line.strip() and 'Welcome' not in line and 'Copyright' not in line:
                print(f"   {line}")
        
        print("\n4. 测试表达式计算...")
        exit_code, stdout, stderr = run_sql("select id, -(col2*(-5)+6)+(col4+3)*(col1+col3*2) as result from exp_table where id=2;")
        print("   输出:")
        for line in (stdout + stderr).split('\n'):
            if line.strip() and 'Welcome' not in line and 'Copyright' not in line:
                print(f"   {line}")
        
        print("\n5. 测试完整查询...")
        exit_code, stdout, stderr = run_sql("select id,-(col2*(-5)+6)+(col4+3)*(col1+col3*2),(9+col2)*col3*9 from exp_table where -(col2*(-6)+5)+(col4+4)*(col1+col3*4) > (2+col2)*col3*1;")
        print("   输出:")
        for line in (stdout + stderr).split('\n'):
            if line.strip() and 'Welcome' not in line and 'Copyright' not in line:
                print(f"   {line}")
        
    finally:
        observer_proc.terminate()
        time.sleep(1)
        observer_proc.kill()
        cleanup()

if __name__ == "__main__":
    main()

