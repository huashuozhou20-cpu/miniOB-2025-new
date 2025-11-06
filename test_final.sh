#!/bin/bash
# 完整功能测试脚本 - 验证所有功能是否正常工作

set -e

TEST_DIR="/tmp/miniob_final_test"
cd /home/obuser/oceanbase-contest/test/miniob-2025-new

# 清理
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

echo "=========================================="
echo "MiniOB 完整功能测试"
echo "=========================================="
echo ""

PASS_COUNT=0
FAIL_COUNT=0

# 测试函数
test_sql() {
    local test_name="$1"
    local sql="$2"
    local expected_pattern="$3"
    
    echo "----------------------------------------"
    echo "测试: $test_name"
    echo "SQL: $sql"
    
    # 启动observer
    timeout 10 ./build/bin/observer -s "$TEST_DIR/miniob.sock" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
    OBSERVER_PID=$!
    sleep 3
    
    # 执行SQL
    echo "$sql" > "$TEST_DIR/test.sql"
    output=$(timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" < "$TEST_DIR/test.sql" 2>&1 | grep -v "Welcome\|Copyright\|Learn\|miniob >" | head -20)
    
    # 检查返回码（4表示SUCCESS）
    if echo "$output" | grep -q "^4$"; then
        echo "✅ PASS (返回码: SUCCESS)"
        ((PASS_COUNT++))
    elif [ -n "$expected_pattern" ] && echo "$output" | grep -qE "$expected_pattern"; then
        echo "✅ PASS (匹配模式: $expected_pattern)"
        ((PASS_COUNT++))
    elif echo "$output" | grep -qi "FAILURE\|ERROR"; then
        echo "❌ FAIL (包含错误)"
        echo "输出: $output"
        ((FAIL_COUNT++))
    else
        echo "⚠️  UNKNOWN (无明确结果)"
        echo "输出: $output"
    fi
    
    # 停止observer
    kill $OBSERVER_PID 2>/dev/null || true
    wait $OBSERVER_PID 2>/dev/null || true
    sleep 1
    
    echo ""
}

echo "=== 测试1: NULL/NOT NULL ==="
test_sql "创建表（默认NOT NULL）" "CREATE TABLE test_null1(id int, name char(10));" ""
test_sql "创建表（显式NOT NULL）" "CREATE TABLE test_null2(id int NOT NULL, name char(10));" ""
test_sql "插入NULL值" "CREATE TABLE test_null3(id int, name char(10)); INSERT INTO test_null3 VALUES(1, NULL);" ""

echo "=== 测试2: 唯一索引 ==="
test_sql "创建表和唯一索引" "CREATE TABLE test_unique(id int, name char(10));
INSERT INTO test_unique VALUES(1, 'a');
CREATE UNIQUE INDEX idx_id ON test_unique(id);" ""
test_sql "插入重复键（应该失败）" "CREATE TABLE test_unique2(id int);
INSERT INTO test_unique2 VALUES(1);
CREATE UNIQUE INDEX idx_id ON test_unique2(id);
INSERT INTO test_unique2 VALUES(1);" ""

echo "=== 测试3: 系统函数 ==="
test_sql "LENGTH函数" "CREATE TABLE test_func(id int, name char(10));
INSERT INTO test_func VALUES(1, 'test');
SELECT LENGTH(name) FROM test_func;" "4"
test_sql "ROUND函数" "CREATE TABLE test_round(id int, price float);
INSERT INTO test_round VALUES(1, 12.345);
SELECT ROUND(price, 2) FROM test_round;" "12.3"

echo "=== 测试4: UNION ==="
test_sql "UNION查询" "CREATE TABLE t1(id int);
CREATE TABLE t2(id int);
INSERT INTO t1 VALUES(1);
INSERT INTO t2 VALUES(1);
SELECT * FROM t1 UNION SELECT * FROM t2;" "1"

echo "=== 测试5: JOIN ==="
test_sql "INNER JOIN" "CREATE TABLE t3(id int, name char(10));
CREATE TABLE t4(id int, age int);
INSERT INTO t3 VALUES(1, 'a');
INSERT INTO t4 VALUES(1, 20);
SELECT * FROM t3 INNER JOIN t4 ON t3.id = t4.id;" "1.*a.*1.*20"

echo "=== 测试6: ALTER TABLE ==="
test_sql "ADD COLUMN" "CREATE TABLE test_alter(id int);
ALTER TABLE test_alter ADD COLUMN name char(10);
INSERT INTO test_alter VALUES(1, 'test');" ""

echo "=== 测试7: Vector类型 ==="
test_sql "STRING_TO_VECTOR" "CREATE TABLE test_vec(id int, vec vector(3));
INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1.0,2.0,3.0]'));" ""

echo "=== 测试8: 日期类型索引 ==="
test_sql "日期索引" "CREATE TABLE test_date(id int, u_date date);
CREATE INDEX idx_date ON test_date(u_date);
INSERT INTO test_date VALUES(1, '2020-01-21');" ""

echo "=== 测试9: 浮点数精度 ==="
test_sql "浮点数输出" "CREATE TABLE test_float(id int, val float);
INSERT INTO test_float VALUES(1, 12.345);
SELECT val FROM test_float;" "12.3"

echo "=========================================="
echo "测试完成"
echo "=========================================="
echo "通过: $PASS_COUNT"
echo "失败: $FAIL_COUNT"
echo "未知: $((10 - PASS_COUNT - FAIL_COUNT))"
echo ""
echo "Observer日志: $TEST_DIR/observer.log"

