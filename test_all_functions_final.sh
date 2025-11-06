#!/bin/bash

# 全面测试所有功能
TEST_DIR="/tmp/miniob_final_test"
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

# 启动observer
echo "=== 启动observer ==="
timeout 30 ./build/bin/observer -s "$TEST_DIR/miniob.sock" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
OBSERVER_PID=$!
sleep 3

PASSED=0
FAILED=0

# 测试函数
test_sql() {
    local name=$1
    local sql=$2
    
    echo ""
    echo "=== 测试: $name ==="
    echo "SQL: $sql"
    
    result=$(echo "$sql" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1 | tail -1)
    exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo "✅ PASS (返回码: $exit_code)"
        ((PASSED++))
        return 0
    else
        echo "❌ FAIL (返回码: $exit_code)"
        echo "输出: $result"
        ((FAILED++))
        return 1
    fi
}

# 1. 测试NOT NULL解析
test_sql "NOT NULL解析" "CREATE TABLE test_notnull(id int not null, age int);"
test_sql "NULL解析" "CREATE TABLE test_null2(id int null, age int);"

# 2. 测试STRING_TO_VECTOR解析和功能
test_sql "CREATE TABLE VECTOR" "CREATE TABLE test_vec(id int, vec vector(3));"
test_sql "INSERT STRING_TO_VECTOR" "INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1.0,2.0,3.0]'));"

# 3. 测试TOKENIZE解析
test_sql "TOKENIZE解析" "SELECT TOKENIZE('hello world', 'jieba') as tokens FROM test_vec LIMIT 1;"

# 4. 测试CREATE UNIQUE INDEX
test_sql "CREATE TABLE for UNIQUE" "CREATE TABLE test_unique(id int, name char(10));"
test_sql "CREATE UNIQUE INDEX" "CREATE UNIQUE INDEX idx_id ON test_unique(id);"
test_sql "INSERT into UNIQUE" "INSERT INTO test_unique VALUES(1, 'test');"

# 5. 测试浮点数精度
test_sql "CREATE TABLE FLOAT" "CREATE TABLE test_float(id int, val float);"
test_sql "INSERT FLOAT" "INSERT INTO test_float VALUES(1, 184.488);"
result=$(echo "SELECT val FROM test_float WHERE id=1;" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1 | grep -E "184\.49|184\.48" | head -1)
if [[ "$result" == *"184.49"* ]]; then
    echo "✅ 浮点数精度 PASS"
    ((PASSED++))
else
    echo "❌ 浮点数精度 FAIL - 结果: $result"
    ((FAILED++))
fi

# 6. 测试创建索引后INSERT
test_sql "CREATE TABLE for INDEX" "CREATE TABLE test_index(id int, name char(10));"
test_sql "CREATE INDEX" "CREATE INDEX idx_id ON test_index(id);"
test_sql "INSERT after INDEX" "INSERT INTO test_index VALUES(1, 'test');"

# 清理
pkill -9 observer 2>/dev/null
wait $OBSERVER_PID 2>/dev/null

echo ""
echo "=========================================="
echo "测试完成"
echo "=========================================="
echo "通过: $PASSED"
echo "失败: $FAILED"
echo "总计: $((PASSED + FAILED))"

