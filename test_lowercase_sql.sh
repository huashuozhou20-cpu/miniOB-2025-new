#!/bin/bash

# MiniOB 小写 SQL 兼容性测试脚本
# 确保所有 SQL 语句的小写形式都能正常工作

OBSERVER_BIN="./build/bin/observer"
OBCLIENT_BIN="./build/bin/obclient"
TEST_DIR="/tmp/miniob_lowercase_test"
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
    echo "MiniOB 小写 SQL 兼容性测试"
    echo "=========================================="
    echo ""
    
    if ! start_observer; then
        exit 1
    fi
    
    echo ""
    
    # ========== 测试 1: 小写 CREATE TABLE ==========
    echo "=========================================="
    echo "测试 1: 小写 CREATE TABLE"
    echo "=========================================="
    run_sql "create table t1(id int, name char, score float);"
    run_sql "create table t2(id int, age int, name char(10), score float);"
    run_sql "create table t3(id int, vec vector(3));"
    run_sql "create table t4(id int, name varchar(50), birthday date);"
    run_sql "create table t5(id int, name string(20), content text);"
    echo ""
    
    # ========== 测试 2: 小写 INSERT ==========
    echo "=========================================="
    echo "测试 2: 小写 INSERT"
    echo "=========================================="
    run_sql "insert into t1 values(1, 'a', 1.0);"
    run_sql "insert into t1 values(2, 'b', 2.0), (3, 'c', 3.0);"
    run_sql "insert into t2 values(1, 10, 'name1', 10.5);"
    echo ""
    
    # ========== 测试 3: 小写 SELECT ==========
    echo "=========================================="
    echo "测试 3: 小写 SELECT"
    echo "=========================================="
    run_sql "select * from t1;"
    run_sql "select id, name, score from t1;"
    run_sql "select * from t1 where id=1;"
    run_sql "select * from t1 where id>=2;"
    run_sql "select * from t1 where id>1 and id<3;"
    run_sql "select count(*) from t1;"
    run_sql "select avg(score) from t1;"
    run_sql "select max(score) from t1;"
    run_sql "select min(score) from t1;"
    run_sql "select sum(score) from t1;"
    echo ""
    
    # ========== 测试 4: 小写 UPDATE ==========
    echo "=========================================="
    echo "测试 4: 小写 UPDATE"
    echo "=========================================="
    run_sql "update t1 set name='updated' where id=1;"
    run_sql "update t1 set score=10.0 where id=2;"
    run_sql "update t1 set name='test' where id>1;"
    echo ""
    
    # ========== 测试 5: 小写 DELETE ==========
    echo "=========================================="
    echo "测试 5: 小写 DELETE"
    echo "=========================================="
    run_sql "delete from t1 where id=3;"
    run_sql "delete from t1 where id=82;"
    echo ""
    
    # ========== 测试 6: 小写 JOIN ==========
    echo "=========================================="
    echo "测试 6: 小写 JOIN"
    echo "=========================================="
    run_sql "create table j1(id int, name char);"
    run_sql "create table j2(id int, num int);"
    run_sql "insert into j1 values(1, 'a'), (2, 'b');"
    run_sql "insert into j2 values(1, 10), (2, 20);"
    run_sql "select * from j1 inner join j2 on j1.id=j2.id;"
    echo ""
    
    # ========== 测试 7: 小写 GROUP BY ==========
    echo "=========================================="
    echo "测试 7: 小写 GROUP BY"
    echo "=========================================="
    run_sql "select id, count(*) from t2 group by id;"
    run_sql "select id, avg(score) from t2 group by id;"
    echo ""
    
    # ========== 测试 8: 小写 ORDER BY ==========
    echo "=========================================="
    echo "测试 8: 小写 ORDER BY"
    echo "=========================================="
    run_sql "select * from t1 order by id;"
    run_sql "select * from t1 order by score desc;"
    echo ""
    
    # ========== 测试 9: 小写 CREATE INDEX ==========
    echo "=========================================="
    echo "测试 9: 小写 CREATE INDEX"
    echo "=========================================="
    run_sql "create index i_id on t1(id);"
    run_sql "create unique index i_name on t1(name);"
    echo ""
    
    # ========== 测试 10: 小写 DROP ==========
    echo "=========================================="
    echo "测试 10: 小写 DROP"
    echo "=========================================="
    run_sql "drop table t3;"
    run_sql "drop table t4;"
    echo ""
    
    # ========== 测试 11: 混合大小写 ==========
    echo "=========================================="
    echo "测试 11: 混合大小写 SQL"
    echo "=========================================="
    run_sql "CREATE table t6(id INT, name CHAR(10));"
    run_sql "INSERT INTO t6 VALUES(1, 'test');"
    run_sql "SELECT * FROM t6 WHERE id=1;"
    run_sql "UPDATE t6 SET name='updated' WHERE id=1;"
    run_sql "DELETE FROM t6 WHERE id=1;"
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
        echo -e "${GREEN}✅ 所有小写 SQL 测试通过！${NC}"
        return 0
    else
        echo -e "${RED}❌ 有小写 SQL 测试失败${NC}"
        return 1
    fi
}

main "$@"

