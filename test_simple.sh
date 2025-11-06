#!/bin/bash
# 简单测试脚本 - 直接查看结果

cd /home/obuser/oceanbase-contest/test/miniob-2025-new

TEST_DIR="/tmp/miniob_test_simple"
SOCKET_FILE="$TEST_DIR/miniob.sock"

# 清理
pkill -9 observer 2>/dev/null
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

echo "启动 observer..."
./build/bin/observer -s "$SOCKET_FILE" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
sleep 3

echo ""
echo "========== 测试1: NULL/NOT NULL 语法 =========="
echo "create table test_null(id int not null, num int null);" | ./build/bin/obclient -s "$SOCKET_FILE" 2>&1 | tail -3

echo ""
echo "========== 测试2: 浮点数精度 =========="
echo "create table exp_test(id int, col1 int, col2 int, col3 float, col4 float);" | ./build/bin/obclient -s "$SOCKET_FILE" 2>&1 | tail -1
echo "insert into exp_test VALUES (2, 3, 7, 8.48, 4.79);" | ./build/bin/obclient -s "$SOCKET_FILE" 2>&1 | tail -1
echo "select -(col2*(-5)+6)+(col4+3)*(col1+col3*2) from exp_test where id=2;" | ./build/bin/obclient -s "$SOCKET_FILE" 2>&1 | grep -E "^[0-9]|^[0-9]\\." | head -5

echo ""
echo "========== 测试3: 查看实际输出 =========="
echo "select id, -(col2*(-5)+6)+(col4+3)*(col1+col3*2) as result from exp_test;" | ./build/bin/obclient -s "$SOCKET_FILE" 2>&1 | tail -10

pkill -9 observer 2>/dev/null

