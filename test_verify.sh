#!/bin/bash
# 实际功能验证测试脚本

set -e

TEST_DIR="/tmp/miniob_verify"
cd /home/obuser/oceanbase-contest/test/miniob-2025-new

# 清理
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

echo "=== MiniOB 功能实际验证测试 ==="
echo ""

# 测试函数
test_sql() {
    local test_name="$1"
    local sql="$2"
    
    echo "----------------------------------------"
    echo "测试: $test_name"
    echo "SQL: $sql"
    echo "----------------------------------------"
    
    # 启动observer
    timeout 10 ./build/bin/observer -s "$TEST_DIR/miniob.sock" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
    OBSERVER_PID=$!
    sleep 3
    
    # 执行SQL并捕获输出
    echo "$sql" > "$TEST_DIR/test.sql"
    output=$(timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" < "$TEST_DIR/test.sql" 2>&1 | grep -v "Welcome\|Copyright\|Learn\|miniob >" | head -20)
    
    # 检查返回码（4表示SUCCESS）
    if echo "$output" | grep -q "^4$"; then
        echo "✅ 返回码: SUCCESS (4)"
    elif echo "$output" | grep -q "SUCCESS"; then
        echo "✅ 输出包含: SUCCESS"
    elif echo "$output" | grep -q "FAILURE\|ERROR"; then
        echo "❌ 输出包含错误"
        echo "输出: $output"
    else
        echo "📋 输出:"
        echo "$output" | head -10
    fi
    
    # 停止observer
    kill $OBSERVER_PID 2>/dev/null || true
    wait $OBSERVER_PID 2>/dev/null || true
    sleep 1
    
    echo ""
}

# 测试1: NULL/NOT NULL
echo "=== 测试1: NULL/NOT NULL ==="
test_sql "创建表（默认NOT NULL）" "CREATE TABLE test_null1(id int, name char(10));"
test_sql "创建表（显式NOT NULL）" "CREATE TABLE test_null2(id int NOT NULL, name char(10));"
test_sql "插入NULL值" "CREATE TABLE test_null3(id int, name char(10)); INSERT INTO test_null3 VALUES(1, NULL);"

# 测试2: 唯一索引
echo "=== 测试2: 唯一索引 ==="
test_sql "创建表和唯一索引" "CREATE TABLE test_unique(id int, name char(10));
INSERT INTO test_unique VALUES(1, 'a');
CREATE UNIQUE INDEX idx_id ON test_unique(id);
INSERT INTO test_unique VALUES(1, 'b');"

# 测试3: 系统函数
echo "=== 测试3: 系统函数 ==="
test_sql "LENGTH函数" "CREATE TABLE test_func(id int, name char(10));
INSERT INTO test_func VALUES(1, 'test');
SELECT LENGTH(name) FROM test_func;"

# 测试4: UNION
echo "=== 测试4: UNION ==="
test_sql "UNION查询" "CREATE TABLE t1(id int);
CREATE TABLE t2(id int);
INSERT INTO t1 VALUES(1);
INSERT INTO t2 VALUES(1);
SELECT * FROM t1 UNION SELECT * FROM t2;"

# 测试5: JOIN
echo "=== 测试5: JOIN ==="
test_sql "INNER JOIN" "CREATE TABLE t3(id int, name char(10));
CREATE TABLE t4(id int, age int);
INSERT INTO t3 VALUES(1, 'a');
INSERT INTO t4 VALUES(1, 20);
SELECT * FROM t3 INNER JOIN t4 ON t3.id = t4.id;"

# 测试6: ALTER TABLE
echo "=== 测试6: ALTER TABLE ==="
test_sql "ADD COLUMN" "CREATE TABLE test_alter(id int);
ALTER TABLE test_alter ADD COLUMN name char(10);
INSERT INTO test_alter VALUES(1, 'test');"

# 测试7: Vector类型
echo "=== 测试7: Vector类型 ==="
test_sql "STRING_TO_VECTOR" "CREATE TABLE test_vec(id int, vec vector(3));
INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1.0,2.0,3.0]'));
SELECT * FROM test_vec;"

# 测试8: 日期类型索引
echo "=== 测试8: 日期类型索引 ==="
test_sql "日期索引" "CREATE TABLE test_date(id int, u_date date);
CREATE INDEX idx_date ON test_date(u_date);
INSERT INTO test_date VALUES(1, '2020-01-21');"

# 测试9: 浮点数精度
echo "=== 测试9: 浮点数精度 ==="
test_sql "浮点数输出" "CREATE TABLE test_float(id int, val float);
INSERT INTO test_float VALUES(1, 12.345);
SELECT val FROM test_float;"

echo "=== 测试完成 ==="
echo "Observer日志位置: $TEST_DIR/observer.log"

