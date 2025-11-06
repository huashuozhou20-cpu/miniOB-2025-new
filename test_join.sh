#!/bin/bash

# 测试 JOIN 功能的脚本

OBSERVER_BIN="./build/bin/observer"
TEST_DIR="/tmp/miniob_test_join"
PORT=6789

# 清理测试目录
rm -rf ${TEST_DIR}
mkdir -p ${TEST_DIR}

# 启动 observer
echo "Starting observer on port ${PORT}..."
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd ${TEST_DIR}
${SCRIPT_DIR}/${OBSERVER_BIN} -p ${PORT} -f ${SCRIPT_DIR}/etc/observer.ini -P mysql -t mvcc -d disk > observer.log 2>&1 &
OBSERVER_PID=$!
cd - > /dev/null

# 等待 observer 启动
echo "Waiting for observer to start..."
sleep 5

# 检查 observer 是否运行
if ! kill -0 $OBSERVER_PID 2>/dev/null; then
    echo "ERROR: Observer failed to start!"
    echo "Observer log:"
    cat ${TEST_DIR}/observer.log
    exit 1
fi

echo "Observer started successfully (PID: $OBSERVER_PID)"

# 使用 mysql 客户端连接并测试
mysql_client() {
    mysql -h 127.0.0.1 -P ${PORT} -u root "$@"
}

echo ""
echo "Creating test tables..."
mysql_client << EOF
CREATE TABLE join_table_1(id int, name char);
CREATE TABLE join_table_2(id int, num int);
CREATE TABLE join_table_3(id int, num2 int);
EOF

echo ""
echo "Inserting test data..."
mysql_client << EOF
INSERT INTO join_table_1 VALUES (1, 'a');
INSERT INTO join_table_1 VALUES (2, 'b');
INSERT INTO join_table_1 VALUES (3, 'c');
INSERT INTO join_table_2 VALUES (1, 2);
INSERT INTO join_table_2 VALUES (2, 15);
INSERT INTO join_table_3 VALUES (1, 120);
INSERT INTO join_table_3 VALUES (3, 800);
EOF

echo ""
echo "=== Test 1: Simple INNER JOIN ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id;"

echo ""
echo "=== Test 2: JOIN with specific columns ==="
mysql_client -e "SELECT join_table_1.name, join_table_2.num FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id;"

echo ""
echo "=== Test 3: Three-table JOIN ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id INNER JOIN join_table_3 ON join_table_1.id=join_table_3.id;"

echo ""
echo "=== Test 4: JOIN with ON condition and WHERE clause ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id AND join_table_2.num>13 WHERE join_table_1.name='b';"

echo ""
echo "=== Test 5: JOIN with multiple ON conditions ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id AND join_table_2.num>13 WHERE join_table_1.name='a';"

# 清理
echo ""
echo "Stopping observer..."
kill $OBSERVER_PID 2>/dev/null
wait $OBSERVER_PID 2>/dev/null

echo "Test completed!"

echo "Creating test tables..."
mysql_client << EOF
CREATE TABLE join_table_1(id int, name char);
CREATE TABLE join_table_2(id int, num int);
CREATE TABLE join_table_3(id int, num2 int);
EOF

echo "Inserting test data..."
mysql_client << EOF
INSERT INTO join_table_1 VALUES (1, 'a');
INSERT INTO join_table_1 VALUES (2, 'b');
INSERT INTO join_table_1 VALUES (3, 'c');
INSERT INTO join_table_2 VALUES (1, 2);
INSERT INTO join_table_2 VALUES (2, 15);
INSERT INTO join_table_3 VALUES (1, 120);
INSERT INTO join_table_3 VALUES (3, 800);
EOF

echo ""
echo "=== Test 1: Simple INNER JOIN ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id;"

echo ""
echo "=== Test 2: JOIN with specific columns ==="
mysql_client -e "SELECT join_table_1.name, join_table_2.num FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id;"

echo ""
echo "=== Test 3: Three-table JOIN ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id INNER JOIN join_table_3 ON join_table_1.id=join_table_3.id;"

echo ""
echo "=== Test 4: JOIN with ON condition and WHERE clause ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id AND join_table_2.num>13 WHERE join_table_1.name='b';"

echo ""
echo "=== Test 5: JOIN with multiple ON conditions ==="
mysql_client -e "SELECT * FROM join_table_1 INNER JOIN join_table_2 ON join_table_1.id=join_table_2.id AND join_table_2.num>13 WHERE join_table_1.name='a';"

# 清理
echo ""
echo "Stopping observer..."
kill $OBSERVER_PID 2>/dev/null
wait $OBSERVER_PID 2>/dev/null

echo "Test completed!"

