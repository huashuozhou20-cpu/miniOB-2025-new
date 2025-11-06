#!/bin/bash

# MiniOB SQL 功能测试脚本
# 使用 Unix socket 连接，符合 miniob 的格式要求

OBSERVER_BIN="./build/bin/observer"
OBCLIENT_BIN="./build/bin/obclient"
TEST_DIR="/tmp/miniob_sql_test"
SOCKET_FILE="$TEST_DIR/miniob.sock"
LOG_FILE="$TEST_DIR/observer.log"

# 颜色输出
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
TOTAL=0

# 清理函数
cleanup() {
    pkill -9 observer 2>/dev/null || true
    rm -rf "$TEST_DIR"
}

# 启动 observer
start_observer() {
    cleanup
    mkdir -p "$TEST_DIR"
    
    echo "启动 observer..."
    $OBSERVER_BIN -s "$SOCKET_FILE" -P mysql -t mvcc -d disk > "$LOG_FILE" 2>&1 &
    OBSERVER_PID=$!
    
    # 等待 socket 文件出现
    for i in {1..15}; do
        if [ -S "$SOCKET_FILE" ]; then
            sleep 2
            echo "✓ Observer 启动成功 (PID: $OBSERVER_PID)"
            return 0
        fi
        sleep 1
    done
    
    echo "✗ Observer 启动失败"
    tail -20 "$LOG_FILE"
    return 1
}

# 执行 SQL 并检查结果
run_sql() {
    local sql="$1"
    local expect_success="${2:-true}"
    
    TOTAL=$((TOTAL + 1))
    
    echo -e "${YELLOW}[测试 $TOTAL]${NC} $sql"
    
    result=$(echo "$sql" | timeout 5 $OBCLIENT_BIN -s "$SOCKET_FILE" 2>&1)
    exit_code=$?
    
    # 检查是否有 SQL_SYNTAX 错误
    if echo "$result" | grep -q "SQL_SYNTAX\|Failed to parse sql"; then
        if [ "$expect_success" = "true" ]; then
            echo -e "${RED}✗ FAIL${NC} - SQL 解析失败"
            echo "  输出: $result" | head -3 | sed 's/^/   /'
            FAILED=$((FAILED + 1))
            return 1
        else
            echo -e "${GREEN}✓ PASS${NC} - 预期失败，实际失败"
            PASSED=$((PASSED + 1))
            return 0
        fi
    fi
    
    # 检查是否有其他错误
    if echo "$result" | grep -qi "error\|fail" && [ "$expect_success" = "true" ]; then
        echo -e "${RED}✗ FAIL${NC} - 执行错误"
        echo "  输出: $result" | head -3 | sed 's/^/   /'
        FAILED=$((FAILED + 1))
        return 1
    fi
    
    if [ "$expect_success" = "true" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        # 显示结果（如果有）
        if echo "$result" | grep -qv "miniob >\|Welcome\|Copyright\|Learn more"; then
            echo "$result" | grep -v "miniob >\|Welcome\|Copyright\|Learn more" | head -5 | sed 's/^/   /'
        fi
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}✗ FAIL${NC} - 预期失败，但执行成功"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

# 主测试函数
main() {
    echo "=========================================="
    echo "MiniOB SQL 功能测试"
    echo "=========================================="
    echo ""
    
    if ! start_observer; then
        exit 1
    fi
    
    echo ""
    
    # ========== 测试 1: CREATE TABLE ==========
    echo "=========================================="
    echo "测试 1: CREATE TABLE"
    echo "=========================================="
    run_sql "create table t_basic(id int, age int, name char, score float);"
    run_sql "CREATE TABLE t_test(id int, name char(10), score float);"
    run_sql "CREATE TABLE t_vector(id int, vec vector(3));"
    echo ""
    
    # ========== 测试 2: INSERT ==========
    echo "=========================================="
    echo "测试 2: INSERT"
    echo "=========================================="
    run_sql "insert into t_basic values(1, 1, 'a', 1.0);"
    run_sql "insert into t_basic values(2, 2, 'b', 2.0);"
    run_sql "insert into t_basic values(3, 3, 'c', 3.0);"
    run_sql "insert into t_basic values(4, 4, 'd', 4.0);"
    run_sql "insert into t_basic values(5, 5, 'e', 5.5);"
    run_sql "insert into t_basic values(6, 6, 'f', 6.6);"
    run_sql "insert into t_basic values(7, 7, 'g', 7.7);"
    run_sql "insert into t_basic values(2, 2, 'b', 2.0), (8, 8, 'h', 8.8);"
    echo ""
    
    # ========== 测试 3: SELECT ==========
    echo "=========================================="
    echo "测试 3: SELECT"
    echo "=========================================="
    run_sql "select * from t_basic;"
    run_sql "select * from t_basic where id=1;"
    run_sql "select * from t_basic where id>=5;"
    run_sql "select * from t_basic where age>1 and age<3;"
    run_sql "select * from t_basic where t_basic.id=1 and t_basic.age=1;"
    run_sql "select id, age, name, score from t_basic;"
    run_sql "select t_basic.id, t_basic.age, t_basic.name from t_basic;"
    echo ""
    
    # ========== 测试 4: UPDATE ==========
    echo "=========================================="
    echo "测试 4: UPDATE"
    echo "=========================================="
    run_sql "update t_basic set name='updated' where id=1;"
    run_sql "select * from t_basic where id=1;"
    run_sql "update t_basic set score=10.0 where id=2;"
    run_sql "update t_basic set age=99 where age=3;"
    run_sql "select * from t_basic where age=99;"
    echo ""
    
    # ========== 测试 5: DELETE ==========
    echo "=========================================="
    echo "测试 5: DELETE"
    echo "=========================================="
    run_sql "delete from t_basic where id=3;"
    run_sql "select * from t_basic where id=3;"
    run_sql "delete from t_basic where id=82;"
    run_sql "select count(*) from t_basic;"
    echo ""
    
    # ========== 测试 6: CREATE INDEX ==========
    echo "=========================================="
    echo "测试 6: CREATE INDEX"
    echo "=========================================="
    run_sql "create index i_id on t_basic(id);"
    run_sql "create index i_age on t_basic(age);"
    echo ""
    
    # ========== 测试 7: JOIN ==========
    echo "=========================================="
    echo "测试 7: JOIN"
    echo "=========================================="
    run_sql "create table t1(id int, name char);"
    run_sql "create table t2(id int, num int);"
    run_sql "insert into t1 values(1, 'a'), (2, 'b');"
    run_sql "insert into t2 values(1, 10), (2, 20);"
    run_sql "select * from t1 inner join t2 on t1.id=t2.id;"
    echo ""
    
    # ========== 测试 8: 聚合函数 ==========
    echo "=========================================="
    echo "测试 8: 聚合函数"
    echo "=========================================="
    run_sql "select count(*) from t_basic;"
    run_sql "select avg(score) from t_basic;"
    run_sql "select max(score) from t_basic;"
    run_sql "select min(score) from t_basic;"
    run_sql "select sum(score) from t_basic;"
    echo ""
    
    # ========== 测试 9: GROUP BY ==========
    echo "=========================================="
    echo "测试 9: GROUP BY"
    echo "=========================================="
    run_sql "select age, count(*) from t_basic group by age;"
    run_sql "select age, avg(score) from t_basic group by age;"
    echo ""
    
    # ========== 测试 10: ORDER BY ==========
    echo "=========================================="
    echo "测试 10: ORDER BY"
    echo "=========================================="
    run_sql "select * from t_basic order by id;"
    run_sql "select * from t_basic order by score desc;"
    echo ""
    
    # ========== 测试 11: DROP TABLE ==========
    echo "=========================================="
    echo "测试 11: DROP TABLE"
    echo "=========================================="
    run_sql "drop table t_test;"
    echo ""
    
    # ========== 测试结果汇总 ==========
    echo "=========================================="
    echo "测试结果汇总"
    echo "=========================================="
    echo "总测试数: $TOTAL"
    echo -e "${GREEN}通过: $PASSED${NC}"
    echo -e "${RED}失败: $FAILED${NC}"
    if [ $TOTAL -gt 0 ]; then
        echo "通过率: $(( PASSED * 100 / TOTAL ))%"
    fi
    echo ""
    
    cleanup
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}✅ 所有测试通过！${NC}"
        return 0
    else
        echo -e "${RED}❌ 有测试失败${NC}"
        return 1
    fi
}

main "$@"
