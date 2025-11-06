#!/bin/bash

# 全面测试所有功能
TEST_DIR="/tmp/miniob_comprehensive_test"
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

# 启动observer
echo "=== 启动observer ==="
timeout 10 ./build/bin/observer -s "$TEST_DIR/miniob.sock" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
OBSERVER_PID=$!
sleep 3

# 测试函数
test_sql() {
    local name=$1
    local sql=$2
    local expected=$3
    
    echo ""
    echo "=== 测试: $name ==="
    echo "SQL: $sql"
    
    result=$(echo "$sql" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1 | tail -1)
    echo "结果: $result"
    
    if [[ "$result" == *"$expected"* ]] || [[ "$expected" == "SUCCESS" && "$result" == "4" ]]; then
        echo "✅ PASS"
        return 0
    else
        echo "❌ FAIL - 期望包含: $expected"
        return 1
    fi
}

# 测试NOT NULL解析
test_sql "NOT NULL解析" "CREATE TABLE test_notnull(id int not null, age int);" "SUCCESS"
test_sql "NULL解析" "CREATE TABLE test_null2(id int null, age int);" "SUCCESS"

# 测试STRING_TO_VECTOR解析
test_sql "STRING_TO_VECTOR解析" "CREATE TABLE test_vec(id int, vec vector(3));" "SUCCESS"
test_sql "STRING_TO_VECTOR插入" "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1.0,2.0,3.0]'));" "SUCCESS"

# 测试TOKENIZE解析
test_sql "TOKENIZE解析" "SELECT TOKENIZE('hello world', 'jieba') as tokens;" "SUCCESS"

# 测试CREATE UNIQUE INDEX
test_sql "CREATE UNIQUE INDEX" "CREATE TABLE test_unique(id int, name char(10));" "SUCCESS"
test_sql "CREATE UNIQUE INDEX创建" "CREATE UNIQUE INDEX idx_id ON test_unique(id);" "SUCCESS"

# 测试浮点数精度
test_sql "浮点数精度测试" "CREATE TABLE test_float(id int, val float);" "SUCCESS"
test_sql "浮点数插入" "INSERT INTO test_float VALUES(1, 184.488);" "SUCCESS"
result=$(echo "SELECT val FROM test_float WHERE id=1;" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1 | grep -E "184\.49|184\.48" | head -1)
if [[ "$result" == *"184.49"* ]]; then
    echo "✅ 浮点数精度 PASS"
else
    echo "❌ 浮点数精度 FAIL - 结果: $result"
fi

# 清理
pkill -9 observer 2>/dev/null
wait $OBSERVER_PID 2>/dev/null

echo ""
echo "=== 测试完成 ==="
