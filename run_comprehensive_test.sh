#!/bin/bash
# 全面测试脚本 - 运行所有功能测试

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "=========================================="
echo "MiniOB 全面功能测试"
echo "=========================================="
echo ""

# 检查编译产物
if [ ! -f "build/bin/observer" ]; then
    echo "错误: observer 未编译"
    echo "请先运行: cd build && make -j4"
    exit 1
fi

if [ ! -f "build/bin/obclient" ]; then
    echo "错误: obclient 未编译"
    exit 1
fi

# 创建工作目录
TEST_DIR="/tmp/miniob_comprehensive_test_$$"
mkdir -p "$TEST_DIR"
echo "测试目录: $TEST_DIR"

# 清理函数
cleanup() {
    echo ""
    echo "清理测试环境..."
    pkill -9 observer 2>/dev/null || true
    sleep 1
    rm -rf "$TEST_DIR"
}

trap cleanup EXIT

# 启动 observer
echo ""
echo "启动 observer..."
./build/bin/observer -s "$TEST_DIR/miniob.sock" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
OBSERVER_PID=$!

echo "Observer PID: $OBSERVER_PID"
sleep 3

# 检查 observer 是否启动成功
if ! ps -p $OBSERVER_PID > /dev/null; then
    echo "错误: observer 启动失败"
    cat "$TEST_DIR/observer.log"
    exit 1
fi

# 等待 socket 文件创建
for i in {1..10}; do
    if [ -S "$TEST_DIR/miniob.sock" ]; then
        echo "✓ Observer socket 已创建"
        break
    fi
    sleep 1
done

if [ ! -S "$TEST_DIR/miniob.sock" ]; then
    echo "错误: socket 文件未创建"
    cat "$TEST_DIR/observer.log"
    exit 1
fi

# 测试函数
run_sql_test() {
    local test_name="$1"
    local sql="$2"
    
    echo ""
    echo "测试: $test_name"
    echo "SQL: $sql"
    
    result=$(echo "$sql" | ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1)
    exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo "✓ 通过"
        return 0
    else
        echo "✗ 失败 (退出码: $exit_code)"
        echo "输出: $result"
        return 1
    fi
}

# 测试统计
PASSED=0
FAILED=0

# 基础功能测试
echo ""
echo "=========================================="
echo "1. 基础功能测试"
echo "=========================================="

# drop-table
run_sql_test "drop-table" "CREATE TABLE test_drop(id INT); DROP TABLE test_drop;" && ((PASSED++)) || ((FAILED++))

# update
run_sql_test "update" "CREATE TABLE test_update(id INT, name CHAR(10)); INSERT INTO test_update VALUES(1, 'test'); UPDATE test_update SET name='updated' WHERE id=1; DROP TABLE test_update;" && ((PASSED++)) || ((FAILED++))

# date
run_sql_test "date" "CREATE TABLE test_date(id INT, birthday DATE); INSERT INTO test_date VALUES(1, '2022-10-10'); DROP TABLE test_date;" && ((PASSED++)) || ((FAILED++))

# expression
run_sql_test "expression" "SELECT 1 + 2 * 3 AS result;" && ((PASSED++)) || ((FAILED++))

# alias
run_sql_test "alias" "CREATE TABLE test_alias(id INT); SELECT id AS num FROM test_alias AS t; DROP TABLE test_alias;" && ((PASSED++)) || ((FAILED++))

# null
run_sql_test "null" "CREATE TABLE test_null(id INT NULL, name CHAR(10) NOT NULL); DROP TABLE test_null;" && ((PASSED++)) || ((FAILED++))

# order-by
run_sql_test "order-by" "CREATE TABLE test_order(id INT, score INT); INSERT INTO test_order VALUES(1, 80), (2, 90); SELECT * FROM test_order ORDER BY score DESC; DROP TABLE test_order;" && ((PASSED++)) || ((FAILED++))

# function
run_sql_test "function" "SELECT LENGTH('hello') AS len, ROUND(3.14) AS rounded;" && ((PASSED++)) || ((FAILED++))

# join-tables
run_sql_test "join-tables" "CREATE TABLE a(id INT, name CHAR); CREATE TABLE b(id INT, num INT); INSERT INTO a VALUES(1, 'x'); INSERT INTO b VALUES(1, 10); SELECT * FROM a INNER JOIN b ON a.id=b.id; DROP TABLE a; DROP TABLE b;" && ((PASSED++)) || ((FAILED++))

# group-by
run_sql_test "group-by" "CREATE TABLE test_group(id INT, score INT); INSERT INTO test_group VALUES(1, 80), (1, 90); SELECT id, AVG(score) FROM test_group GROUP BY id; DROP TABLE test_group;" && ((PASSED++)) || ((FAILED++))

# multi-index
run_sql_test "multi-index" "CREATE TABLE test_multi(id INT, col1 INT, col2 INT); CREATE INDEX i_multi ON test_multi(col1, col2); DROP TABLE test_multi;" && ((PASSED++)) || ((FAILED++))

# unique
run_sql_test "unique" "CREATE TABLE test_unique(id INT); CREATE UNIQUE INDEX i_unique ON test_unique(id); DROP TABLE test_unique;" && ((PASSED++)) || ((FAILED++))

# text
run_sql_test "text" "CREATE TABLE test_text(id INT, article TEXT); DROP TABLE test_text;" && ((PASSED++)) || ((FAILED++))

# union
run_sql_test "union" "CREATE TABLE t1(id INT); CREATE TABLE t2(id INT); INSERT INTO t1 VALUES(1); INSERT INTO t2 VALUES(1); SELECT * FROM t1 UNION SELECT * FROM t2; DROP TABLE t1; DROP TABLE t2;" && ((PASSED++)) || ((FAILED++))

# vector-basic
run_sql_test "vector-basic" "CREATE TABLE test_vec(id INT, vec VECTOR(3)); INSERT INTO test_vec VALUES(1, STRING_TO_VECTOR('[1,2,3]')); SELECT DISTANCE(STRING_TO_VECTOR('[1,2,3]'), STRING_TO_VECTOR('[2,3,4]'), 'COSINE') AS dist; DROP TABLE test_vec;" && ((PASSED++)) || ((FAILED++))

# alter
run_sql_test "alter" "CREATE TABLE test_alter(id INT); ALTER TABLE test_alter ADD COLUMN name CHAR(10); ALTER TABLE test_alter DROP COLUMN name; ALTER TABLE test_alter RENAME TO test_alter2; DROP TABLE test_alter2;" && ((PASSED++)) || ((FAILED++))

# 总结
echo ""
echo "=========================================="
echo "测试总结"
echo "=========================================="
echo "通过: $PASSED"
echo "失败: $FAILED"
echo "总计: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo ""
    echo "✓ 所有测试通过！"
    exit 0
else
    echo ""
    echo "✗ 部分测试失败"
    exit 1
fi

