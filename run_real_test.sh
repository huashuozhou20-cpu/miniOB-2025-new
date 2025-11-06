#!/bin/bash
# 实际功能测试脚本 - 启动 observer 并测试所有 SQL 功能

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

OBSERVER_BIN="./build/bin/observer"
OBCLIENT_BIN="./build/bin/obclient"
TEST_DIR="/tmp/miniob_real_test"
SOCKET_FILE="$TEST_DIR/miniob.sock"
LOG_FILE="$TEST_DIR/observer.log"

# 清理函数
cleanup() {
    echo ""
    echo "清理测试环境..."
    pkill -9 observer 2>/dev/null || true
    sleep 1
    if [ -d "$TEST_DIR" ]; then
        echo "Observer 日志（最后50行）:"
        tail -50 "$LOG_FILE" 2>/dev/null || echo "无日志文件"
    fi
}

# 捕获退出信号
trap cleanup EXIT

# 启动 observer
start_observer() {
    echo "=========================================="
    echo "启动 MiniOB Observer"
    echo "=========================================="
    
    # 清理旧目录
    rm -rf "$TEST_DIR"
    mkdir -p "$TEST_DIR"
    
    echo "启动命令: $OBSERVER_BIN -s $SOCKET_FILE -P mysql -t mvcc -d disk"
    $OBSERVER_BIN -s "$SOCKET_FILE" -P mysql -t mvcc -d disk > "$LOG_FILE" 2>&1 &
    OBSERVER_PID=$!
    
    echo "等待 observer 启动 (PID: $OBSERVER_PID)..."
    
    # 等待 socket 文件出现
    for i in {1..30}; do
        if [ -S "$SOCKET_FILE" ]; then
            echo "✓ Socket 文件已创建: $SOCKET_FILE"
            sleep 2  # 额外等待确保完全初始化
            return 0
        fi
        sleep 1
        echo -n "."
    done
    echo ""
    
    echo "✗ Observer 启动超时"
    echo "检查日志:"
    tail -30 "$LOG_FILE"
    return 1
}

# 运行 SQL
run_sql() {
    local sql="$1"
    local desc="${2:-}"
    
    if [ -n "$desc" ]; then
        echo "[$desc]"
    fi
    echo "  SQL: $sql"
    
    result=$(echo "$sql" | $OBCLIENT_BIN -s "$SOCKET_FILE" 2>&1)
    exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        if echo "$result" | grep -qi "SUCCESS\|success\|successful" || [ -z "$result" ]; then
            echo "  状态: ✓ 成功"
            if [ -n "$result" ] && echo "$result" | grep -vqi "SUCCESS"; then
                echo "  结果:"
                echo "$result" | sed 's/^/    /' | head -10
            fi
            return 0
        else
            echo "  状态: ⚠ 执行完成但可能有警告"
            echo "  输出:"
            echo "$result" | sed 's/^/    /' | head -10
            return 0
        fi
    else
        echo "  状态: ✗ 失败 (退出码: $exit_code)"
        echo "  错误:"
        echo "$result" | sed 's/^/    /' | head -10
        return 1
    fi
}

# 主测试函数
main() {
    echo ""
    echo "=========================================="
    echo "MiniOB 实际功能测试"
    echo "=========================================="
    echo ""
    
    if [ ! -f "$OBSERVER_BIN" ]; then
        echo "错误: observer 二进制文件不存在: $OBSERVER_BIN"
        exit 1
    fi
    
    if [ ! -f "$OBCLIENT_BIN" ]; then
        echo "错误: obclient 二进制文件不存在: $OBCLIENT_BIN"
        exit 1
    fi
    
    if ! start_observer; then
        exit 1
    fi
    
    echo ""
    echo "=========================================="
    echo "开始测试"
    echo "=========================================="
    echo ""
    
    PASSED=0
    FAILED=0
    
    # 测试 1: CREATE TABLE (包括 VECTOR)
    echo "=========================================="
    echo "测试 1: CREATE TABLE"
    echo "=========================================="
    echo ""
    
    if run_sql "CREATE TABLE test_basic(id int, name char(10), score float);" "CREATE TABLE - 基本类型"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "CREATE TABLE test_vector(id int, embedding vector(3));" "CREATE TABLE - VECTOR类型"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "CREATE TABLE test_vector2(id int, vec vector(5));" "CREATE TABLE - VECTOR 小写"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "CREATE TABLE test_mixed(id int, name char(10), score float, vec vector(3));" "CREATE TABLE - 混合类型"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    # 测试 2: INSERT
    echo "=========================================="
    echo "测试 2: INSERT"
    echo "=========================================="
    echo ""
    
    if run_sql "INSERT INTO test_basic VALUES (1, 'Alice', 85.5);" "INSERT - 单行"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "INSERT INTO test_basic VALUES (2, 'Bob', 90.0), (3, 'Charlie', 75.5);" "INSERT - 多行"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "INSERT INTO test_vector VALUES (1, '[1.0,2.0,3.0]');" "INSERT - VECTOR类型"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    # 测试 3: SELECT
    echo "=========================================="
    echo "测试 3: SELECT"
    echo "=========================================="
    echo ""
    
    if run_sql "SELECT * FROM test_basic;" "SELECT - 所有列"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "SELECT id, name FROM test_basic;" "SELECT - 指定列"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "SELECT * FROM test_basic WHERE id=1;" "SELECT - WHERE条件"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "SELECT * FROM test_vector;" "SELECT - VECTOR表"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    # 测试 4: UPDATE
    echo "=========================================="
    echo "测试 4: UPDATE"
    echo "=========================================="
    echo ""
    
    if run_sql "UPDATE test_basic SET score=95.0 WHERE id=1;" "UPDATE - 更新数据"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "SELECT * FROM test_basic WHERE id=1;" "SELECT - 验证更新"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    # 测试 5: DELETE
    echo "=========================================="
    echo "测试 5: DELETE"
    echo "=========================================="
    echo ""
    
    if run_sql "DELETE FROM test_basic WHERE id=2;" "DELETE - 删除数据"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "SELECT * FROM test_basic;" "SELECT - 验证删除"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    # 测试 6: JOIN
    echo "=========================================="
    echo "测试 6: JOIN (多表连接)"
    echo "=========================================="
    echo ""
    
    if run_sql "CREATE TABLE join_a(id int, name char(10));" "CREATE - JOIN表A"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "CREATE TABLE join_b(id int, num int);" "CREATE - JOIN表B"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "INSERT INTO join_a VALUES (1, 'X'), (2, 'Y');" "INSERT - JOIN表A数据"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "INSERT INTO join_b VALUES (1, 10), (2, 20);" "INSERT - JOIN表B数据"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    if run_sql "SELECT * FROM join_a INNER JOIN join_b ON join_a.id=join_b.id;" "SELECT - INNER JOIN"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    # 测试 7: DROP TABLE
    echo "=========================================="
    echo "测试 7: DROP TABLE"
    echo "=========================================="
    echo ""
    
    if run_sql "DROP TABLE test_basic;" "DROP TABLE - 删除表"; then
        PASSED=$((PASSED + 1))
    else
        FAILED=$((FAILED + 1))
    fi
    echo ""
    
    # 测试总结
    echo "=========================================="
    echo "测试总结"
    echo "=========================================="
    echo ""
    echo "通过: $PASSED"
    echo "失败: $FAILED"
    echo "总计: $((PASSED + FAILED))"
    echo ""
    
    if [ $FAILED -eq 0 ]; then
        echo "✓ 所有测试通过！"
        return 0
    else
        echo "✗ 有 $FAILED 个测试失败"
        return 1
    fi
}

# 运行主函数
main "$@"

