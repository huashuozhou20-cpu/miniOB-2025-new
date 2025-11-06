#!/bin/bash
# 使用报错日志中的SQL语句进行测试 - 改进版

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

OBSERVER_BIN="./build/bin/observer"
OBCLIENT_BIN="./build/bin/obclient"
TEST_DIR="/tmp/miniob_sql_test"
SOCKET_FILE="$TEST_DIR/miniob.sock"

# 清理
cleanup() {
    pkill -9 observer 2>/dev/null
    rm -rf "$TEST_DIR"
    mkdir -p "$TEST_DIR"
}

# 启动 observer
start_observer() {
    cleanup
    echo "启动 observer..."
    $OBSERVER_BIN -s "$SOCKET_FILE" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
    OBSERVER_PID=$!
    
    # 等待 socket 文件出现
    for i in {1..20}; do
        if [ -S "$SOCKET_FILE" ]; then
            sleep 2
            echo "✓ Observer 启动成功 (PID: $OBSERVER_PID)"
            return 0
        fi
        sleep 1
    done
    
    echo "✗ Observer 启动失败"
    cat "$TEST_DIR/observer.log" | tail -30
    return 1
}

# 运行 SQL - 返回退出码和输出
run_sql() {
    local sql="$1"
    echo "$sql" | $OBCLIENT_BIN -s "$SOCKET_FILE" > "$TEST_DIR/sql_output.txt" 2>&1
    local exit_code=$?
    cat "$TEST_DIR/sql_output.txt"
    return $exit_code
}

# 测试用例
test_sql() {
    local name="$1"
    local sql="$2"
    local expected="${3:-SUCCESS}"
    
    echo ""
    echo "=========================================="
    echo "测试: $name"
    echo "SQL: $sql"
    echo "------------------------------------------"
    
    run_sql "$sql" > "$TEST_DIR/result.txt"
    exit_code=$?
    result=$(cat "$TEST_DIR/result.txt")
    
    # 过滤掉欢迎信息
    result=$(echo "$result" | grep -v "Welcome to\|Copyright\|Learn more\|miniob >" | sed 's/^[0-9]*miniob > //')
    
    echo "$result"
    echo "------------------------------------------"
    echo "退出码: $exit_code"
    
    # obclient返回码: 0=成功, 4=成功(可能是), 非0=失败
    # 检查输出中是否有错误信息
    if echo "$result" | grep -qi "ERROR\|FAIL\|error\|fail\|SQL_SYNTAX\|Failed to parse"; then
        if [ "$expected" = "FAILURE" ]; then
            echo "✓ PASS (期望失败，实际失败)"
            return 0
        else
            echo "✗ FAIL (期望成功，但实际失败)"
            return 1
        fi
    elif [ $exit_code -eq 0 ] || [ $exit_code -eq 4 ]; then
        if [ "$expected" = "SUCCESS" ]; then
            echo "✓ PASS (期望成功，实际成功)"
            return 0
        else
            echo "✗ FAIL (期望失败，但实际成功)"
            return 1
        fi
    else
        echo "? UNKNOWN (退出码: $exit_code)"
        return 2
    fi
}

# 测试查询结果
test_query() {
    local name="$1"
    local sql="$2"
    local expected_values="$3"
    
    echo ""
    echo "=========================================="
    echo "测试: $name"
    echo "SQL: $sql"
    echo "------------------------------------------"
    
    run_sql "$sql" > "$TEST_DIR/query_result.txt"
    exit_code=$?
    result=$(cat "$TEST_DIR/query_result.txt")
    
    # 过滤掉欢迎信息
    result=$(echo "$result" | grep -v "Welcome to\|Copyright\|Learn more\|miniob >" | sed 's/^[0-9]*miniob > //')
    
    echo "$result"
    echo "------------------------------------------"
    
    if [ $exit_code -ne 0 ] && [ $exit_code -ne 4 ]; then
        echo "✗ FAIL (查询执行失败，退出码: $exit_code)"
        return 1
    fi
    
    # 检查是否包含期望的值
    all_found=true
    for value in $expected_values; do
        if echo "$result" | grep -q "$value"; then
            echo "✓ 找到期望值: $value"
        else
            echo "✗ 未找到期望值: $value"
            all_found=false
        fi
    done
    
    if [ "$all_found" = true ]; then
        echo "✓ PASS (所有期望值都找到)"
        return 0
    else
        echo "✗ FAIL (部分期望值未找到)"
        return 1
    fi
}

# 主测试
main() {
    echo "=========================================="
    echo "MiniOB SQL 修复验证测试"
    echo "使用报错日志中的SQL语句"
    echo "=========================================="
    
    if [ ! -f "$OBSERVER_BIN" ]; then
        echo "错误: observer 可执行文件不存在: $OBSERVER_BIN"
        exit 1
    fi
    
    if [ ! -f "$OBCLIENT_BIN" ]; then
        echo "错误: obclient 可执行文件不存在: $OBCLIENT_BIN"
        exit 1
    fi
    
    if ! start_observer; then
        exit 1
    fi
    
    # 等待一下确保完全启动
    sleep 1
    
    PASSED=0
    FAILED=0
    
    # 测试1: NULL/NOT NULL 语法 - create-view
    echo ""
    echo "========== 测试1: NULL/NOT NULL 语法 (create-view) =========="
    if test_sql "create-view" "create table create_view_t2(id int not null, age int, name char(10));" "SUCCESS"; then
        ((PASSED++))
    else
        ((FAILED++))
    fi
    
    # 测试2: NULL/NOT NULL 语法 - group-by
    echo ""
    echo "========== 测试2: NULL/NOT NULL 语法 (group-by) =========="
    if test_sql "group-by" "create table t_group_by (id int not null, score int not null, name char(1) null);" "SUCCESS"; then
        ((PASSED++))
    else
        ((FAILED++))
    fi
    
    # 测试3: NULL/NOT NULL 语法 - null
    echo ""
    echo "========== 测试3: NULL/NOT NULL 语法 (null) =========="
    if test_sql "null" "CREATE TABLE null_table(id int not null, num int null, price float not null, birthday date null);" "SUCCESS"; then
        ((PASSED++))
    else
        ((FAILED++))
    fi
    
    # 测试4: NULL/NOT NULL 语法 - order-by
    echo ""
    echo "========== 测试4: NULL/NOT NULL 语法 (order-by) =========="
    if test_sql "order-by" "create table t_order_by_3(id int null, score float);" "SUCCESS"; then
        ((PASSED++))
    else
        ((FAILED++))
    fi
    
    # 测试5: 浮点数精度 - expression
    echo ""
    echo "========== 测试5: 浮点数精度 (expression) =========="
    run_sql "create table exp_table(id int, col1 int, col2 int, col3 float, col4 float);" > /dev/null 2>&1
    run_sql "insert into exp_table VALUES (8, 8, 9, 8.01, 3.15);" > /dev/null 2>&1
    run_sql "insert into exp_table VALUES (3, 8, 6, 5.12, 9.94);" > /dev/null 2>&1
    run_sql "insert into exp_table VALUES (2, 3, 7, 8.48, 4.79);" > /dev/null 2>&1
    
    if test_query "expression" "select id,-(col2*(-5)+6)+(col4+3)*(col1+col3*2),(9+col2)*col3*9 from exp_table where -(col2*(-6)+5)+(col4+4)*(col1+col3*4) > (2+col2)*col3*1;" "184.49 260.03 186.72"; then
        ((PASSED++))
    else
        ((FAILED++))
    fi
    
    # 清理
    cleanup
    
    echo ""
    echo "=========================================="
    echo "测试完成"
    echo "通过: $PASSED"
    echo "失败: $FAILED"
    echo "=========================================="
    
    if [ $FAILED -eq 0 ]; then
        exit 0
    else
        exit 1
    fi
}

main "$@"
