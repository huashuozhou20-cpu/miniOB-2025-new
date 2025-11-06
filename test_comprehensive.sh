#!/bin/bash
# 全面功能测试脚本
# 测试所有已实现的功能是否能在miniob上正常工作

set -e

TEST_DIR="/tmp/miniob_comprehensive_test"
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

OBSERVER_LOG="$TEST_DIR/observer.log"
CLIENT_LOG="$TEST_DIR/client.log"

echo "=== MiniOB 功能全面测试 ==="
echo ""

# 启动observer
echo "1. 启动observer..."
cd /home/obuser/oceanbase-contest/test/miniob-2025-new
timeout 30 ./build/bin/observer -s "$TEST_DIR/miniob.sock" -P mysql -t mvcc -d disk > "$OBSERVER_LOG" 2>&1 &
OBSERVER_PID=$!
sleep 3

# 检查observer是否启动成功
if ! ps -p $OBSERVER_PID > /dev/null; then
    echo "❌ Observer启动失败"
    cat "$OBSERVER_LOG"
    exit 1
fi

echo "✅ Observer已启动 (PID: $OBSERVER_PID)"
echo ""

# 测试函数
test_sql() {
    local test_name="$1"
    local sql="$2"
    local expected="$3"
    
    echo "测试: $test_name"
    echo "SQL: $sql"
    
    result=$(echo "$sql" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1 | grep -v "Welcome\|Copyright\|Learn\|miniob >" | head -5)
    
    if echo "$result" | grep -q "$expected"; then
        echo "✅ PASS"
    else
        echo "❌ FAIL"
        echo "期望包含: $expected"
        echo "实际输出: $result"
    fi
    echo ""
}

# 测试1: NULL/NOT NULL
echo "=== 测试1: NULL/NOT NULL ==="
test_sql "创建表（默认NOT NULL）" "CREATE TABLE test_null1(id int, name char(10));" "SUCCESS"
test_sql "创建表（显式NOT NULL）" "CREATE TABLE test_null2(id int NOT NULL, name char(10));" "SUCCESS"
test_sql "插入NULL值" "INSERT INTO test_null1 VALUES(1, NULL);" "SUCCESS"
test_sql "查询NULL值" "SELECT * FROM test_null1;" "1.*NULL"

# 测试2: 唯一索引
echo "=== 测试2: 唯一索引 ==="
test_sql "创建表" "CREATE TABLE test_unique(id int, name char(10));" "SUCCESS"
test_sql "插入第一条记录" "INSERT INTO test_unique VALUES(1, 'a');" "SUCCESS"
test_sql "创建唯一索引" "CREATE UNIQUE INDEX idx_id ON test_unique(id);" "SUCCESS"
test_sql "插入重复键（应该失败）" "INSERT INTO test_unique VALUES(1, 'b');" "DUPLICATE\|FAILURE"

# 测试3: 系统函数
echo "=== 测试3: 系统函数 ==="
test_sql "创建表" "CREATE TABLE test_func(id int, name char(10), price float);" "SUCCESS"
test_sql "插入数据" "INSERT INTO test_func VALUES(1, 'test', 12.345);" "SUCCESS"
test_sql "测试LENGTH" "SELECT LENGTH(name) FROM test_func;" "4"
test_sql "测试ROUND" "SELECT ROUND(price, 2) FROM test_func;" "12.35\|12.34"

# 测试4: UNION
echo "=== 测试4: UNION ==="
test_sql "创建表1" "CREATE TABLE t1(id int);" "SUCCESS"
test_sql "创建表2" "CREATE TABLE t2(id int);" "SUCCESS"
test_sql "插入数据" "INSERT INTO t1 VALUES(1); INSERT INTO t2 VALUES(1);" "SUCCESS"
test_sql "UNION查询" "SELECT * FROM t1 UNION SELECT * FROM t2;" "1"

# 测试5: JOIN
echo "=== 测试5: JOIN ==="
test_sql "创建表" "CREATE TABLE t3(id int, name char(10)); CREATE TABLE t4(id int, age int);" "SUCCESS"
test_sql "插入数据" "INSERT INTO t3 VALUES(1, 'a'); INSERT INTO t4 VALUES(1, 20);" "SUCCESS"
test_sql "INNER JOIN" "SELECT * FROM t3 INNER JOIN t4 ON t3.id = t4.id;" "1.*a.*1.*20"

# 测试6: ALTER TABLE
echo "=== 测试6: ALTER TABLE ==="
test_sql "创建表" "CREATE TABLE test_alter(id int);" "SUCCESS"
test_sql "添加列" "ALTER TABLE test_alter ADD COLUMN name char(10);" "SUCCESS"
test_sql "插入数据" "INSERT INTO test_alter VALUES(1, 'test');" "SUCCESS"

# 测试7: Vector类型
echo "=== 测试7: Vector类型 ==="
test_sql "创建向量表" "CREATE TABLE test_vec(id int, vec vector(3));" "SUCCESS"
test_sql "插入向量" "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1.0,2.0,3.0]'));" "SUCCESS"
test_sql "查询向量" "SELECT * FROM test_vec;" "1.*vector"

# 测试8: 日期类型索引
echo "=== 测试8: 日期类型索引 ==="
test_sql "创建日期表" "CREATE TABLE test_date(id int, u_date date);" "SUCCESS"
test_sql "创建索引" "CREATE INDEX idx_date ON test_date(u_date);" "SUCCESS"
test_sql "插入日期" "INSERT INTO test_date VALUES(1, '2020-01-21');" "SUCCESS"

# 测试9: TOKENIZE
echo "=== 测试9: TOKENIZE ==="
test_sql "测试TOKENIZE" "SELECT TOKENIZE('hello world', 'jieba') as tokens;" "tokens"

# 测试10: 浮点数精度
echo "=== 测试10: 浮点数精度 ==="
test_sql "创建表" "CREATE TABLE test_float(id int, val float);" "SUCCESS"
test_sql "插入数据" "INSERT INTO test_float VALUES(1, 12.345);" "SUCCESS"
test_sql "查询浮点数" "SELECT val FROM test_float;" "12.35\|12.34"

# 清理
echo "=== 清理 ==="
kill $OBSERVER_PID 2>/dev/null || true
wait $OBSERVER_PID 2>/dev/null || true

echo ""
echo "=== 测试完成 ==="
echo "Observer日志: $OBSERVER_LOG"
echo "Client日志: $CLIENT_LOG"

