#!/bin/bash
PORT=6789
OBSERVER_BIN="./build/bin/observer"
OBCLIENT_BIN="./build/bin/obclient"

# 启动 observer
$OBSERVER_BIN -p $PORT -P mysql -t mvcc -d disk > /tmp/observer_basic.log 2>&1 &
OBSERVER_PID=$!
sleep 10

# 测试 SQL
TEST_SQL="CREATE TABLE t_basic(id int, age int, name char(4), score float);"
echo "测试 SQL: $TEST_SQL"
echo "$TEST_SQL" | $OBCLIENT_BIN -h 127.0.0.1 -p $PORT 2>&1

# 清理
pkill -9 observer 2>/dev/null || true
