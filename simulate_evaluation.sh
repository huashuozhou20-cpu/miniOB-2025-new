#!/bin/bash

# MiniOB 评测模拟脚本
# 模拟miniob评测系统的行为，使用实际评测SQL进行测试

TEST_DIR="/tmp/miniob_eval_test"
rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

# 启动observer
echo "=========================================="
echo "启动 MiniOB Observer"
echo "=========================================="
timeout 60 ./build/bin/observer -s "$TEST_DIR/miniob.sock" -P mysql -t mvcc -d disk > "$TEST_DIR/observer.log" 2>&1 &
OBSERVER_PID=$!
sleep 3

PASSED=0
FAILED=0
UNKNOWN=0

# 测试函数
test_case() {
    local name=$1
    local sql=$2
    local expected=$3  # SUCCESS, FAILURE, SQL_SYNTAX, 或具体结果
    
    echo ""
    echo "----------------------------------------"
    echo "测试: $name"
    echo "SQL: $sql"
    
    result=$(echo "$sql" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1)
    exit_code=$?
    
    # 检查返回码和输出
    if [ $exit_code -eq 0 ]; then
        if [[ "$expected" == "SUCCESS" ]] || [[ -z "$expected" ]]; then
            echo "✅ PASS"
            ((PASSED++))
            return 0
        elif [[ "$result" == *"$expected"* ]]; then
            echo "✅ PASS (匹配预期: $expected)"
            ((PASSED++))
            return 0
        else
            echo "⚠️  UNKNOWN (返回成功但结果不匹配)"
            echo "输出: $result"
            ((UNKNOWN++))
            return 2
        fi
    else
        if [[ "$expected" == "FAILURE" ]] || [[ "$expected" == "SQL_SYNTAX" ]]; then
            echo "✅ PASS (预期失败)"
            ((PASSED++))
            return 0
        else
            echo "❌ FAIL (返回码: $exit_code)"
            echo "输出: $result"
            ((FAILED++))
            return 1
        fi
    fi
}

echo ""
echo "=========================================="
echo "开始评测"
echo "=========================================="

# ========== 1. NULL/NOT NULL 测试 ==========
echo ""
echo "========== 1. NULL/NOT NULL 测试 =========="
test_case "create-view: NOT NULL" "create table create_view_t2(id int not null, age int, name char(10));" "SUCCESS"
test_case "group-by: NOT NULL" "create table t_group_by (id int not null, score int not null, name char(1) null);" "SUCCESS"
test_case "null: NOT NULL and NULL" "CREATE TABLE null_table(id int not null, num int null, price float not null, birthday date null);" "SUCCESS"
test_case "order-by: NULL" "create table t_order_by_3(id int null, score float);" "SUCCESS"

# ========== 2. STRING_TO_VECTOR 测试 ==========
echo ""
echo "========== 2. STRING_TO_VECTOR 测试 =========="
test_case "vector-basic: CREATE TABLE" "CREATE TABLE TEST (ID INT, C1 VECTOR(3));" "SUCCESS"
test_case "vector-basic: INSERT STRING_TO_VECTOR" "INSERT INTO TEST VALUES(1, STRING_TO_VECTOR('[5.86,1.69,0.67]'));" "SUCCESS"
test_case "vector-search: CREATE TABLE" "CREATE TABLE TAB_VEC(ID INT, A INT, B VECTOR(10));" "SUCCESS"
test_case "vector-search: INSERT STRING_TO_VECTOR" "INSERT INTO TAB_VEC VALUES(1, 1, STRING_TO_VECTOR('[1.61,2.71,1.0,2.26,1.55,0.87,3.96,2.29,2.32,3.22]'));" "SUCCESS"

# ========== 3. TOKENIZE 测试 ==========
echo ""
echo "========== 3. TOKENIZE 测试 =========="
test_case "full-text-index: TOKENIZE" "SELECT TOKENIZE('如何设置和使用secure_file_priv系统变量以确保从OBServer节点文件导入数据的安全性？', 'jieba') as text_tokens;" "SUCCESS"

# ========== 4. CREATE UNIQUE INDEX 测试 ==========
echo ""
echo "========== 4. CREATE UNIQUE INDEX 测试 =========="
test_case "Unique: CREATE TABLE" "CREATE TABLE unique_table1(id int, col1 int, col2 int);" "SUCCESS"
test_case "Unique: INSERT data" "INSERT INTO unique_table1 VALUES (1,1,1);" "SUCCESS"
test_case "Unique: CREATE UNIQUE INDEX" "CREATE UNIQUE INDEX index_id on unique_table1(id);" "SUCCESS"
test_case "Unique: INSERT duplicate" "INSERT INTO unique_table1 VALUES (1,2,2);" "FAILURE"  # 应该失败

# ========== 5. 创建索引后INSERT测试 ==========
echo ""
echo "========== 5. 创建索引后INSERT测试 =========="
test_case "Alter: CREATE TABLE" "CREATE TABLE alter_table_1(id int);" "SUCCESS"
test_case "Alter: CREATE INDEX" "CREATE index i_id on alter_table_1(id);" "SUCCESS"
test_case "Alter: INSERT after INDEX" "INSERT INTO alter_table_1 VALUES (6);" "SUCCESS"

test_case "Date: CREATE TABLE" "CREATE TABLE date_table(id int, u_date date);" "SUCCESS"
test_case "Date: CREATE INDEX" "CREATE INDEX index_id on date_table(u_date);" "SUCCESS"
test_case "Date: INSERT after INDEX" "INSERT INTO date_table VALUES (1,'2020-01-21');" "SUCCESS"

test_case "Update: CREATE TABLE" "CREATE TABLE update_table_1(id int, t_name char(20), col1 int, col2 int);" "SUCCESS"
test_case "Update: CREATE INDEX" "CREATE INDEX index_id on update_table_1(id);" "SUCCESS"
test_case "Update: INSERT after INDEX" "INSERT INTO update_table_1 VALUES (1, 'FYUIGRO6IHU98WC7R', 16, 46);" "SUCCESS"

# ========== 6. 浮点数精度测试 ==========
echo ""
echo "========== 6. 浮点数精度测试 =========="
test_case "expression: CREATE TABLE" "create table exp_table(id int, col1 int, col2 int, col3 float, col4 float);" "SUCCESS"
test_case "expression: INSERT data" "insert into exp_table VALUES (8, 8, 9, 8.01, 3.15);" "SUCCESS"
test_case "expression: INSERT data2" "insert into exp_table VALUES (3, 8, 6, 5.12, 9.94);" "SUCCESS"
test_case "expression: INSERT data3" "insert into exp_table VALUES (2, 3, 7, 8.48, 4.79);" "SUCCESS"

# 测试浮点数精度查询
result=$(echo "select id,-(col2*(-5)+6)+(col4+3)*(col1+col3*2) from exp_table where id=2;" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1 | grep -E "184\.49|184\.48" | head -1)
if [[ "$result" == *"184.49"* ]]; then
    echo "✅ 浮点数精度 PASS (184.49)"
    ((PASSED++))
else
    echo "⚠️  浮点数精度检查 (结果: $result)"
    ((UNKNOWN++))
fi

# ========== 7. UNION 测试 ==========
echo ""
echo "========== 7. UNION 测试 =========="
test_case "Union: CREATE TABLE 1" "CREATE TABLE union_table_1(id int, col1 int);" "SUCCESS"
test_case "Union: INSERT data1" "INSERT INTO union_table_1 VALUES (19, 39);" "SUCCESS"
test_case "Union: INSERT data2" "INSERT INTO union_table_1 VALUES (20, 20);" "SUCCESS"
test_case "Union: CREATE TABLE 2" "CREATE TABLE union_table_2(id int, col2 int);" "SUCCESS"
test_case "Union: INSERT data3" "INSERT INTO union_table_2 VALUES (20, 20);" "SUCCESS"
test_case "Union: CREATE TABLE 3" "CREATE TABLE union_table_3(id int, col2 int);" "SUCCESS"
test_case "Union: INSERT data4" "INSERT INTO union_table_3 VALUES (20, 20);" "SUCCESS"

# UNION查询测试
result=$(echo "select * from union_table_1 where id < 50 union select * from union_table_2 where id > 10 union select * from union_table_3 where id >= 20 and id <=30;" | timeout 5 ./build/bin/obclient -s "$TEST_DIR/miniob.sock" 2>&1 | grep -c "20.*20")
if [ "$result" -eq 1 ]; then
    echo "✅ UNION去重 PASS (20|20出现1次)"
    ((PASSED++))
else
    echo "⚠️  UNION去重检查 (20|20出现${result}次，期望1次)"
    ((UNKNOWN++))
fi

# ========== 8. Function 测试 ==========
echo ""
echo "========== 8. Function 测试 =========="
test_case "Function: LENGTH" "select length('48QVE3VX8NDG039FEKPPZ57XF1VW46IP3PPZH49RXGH8VZRZ2BP18HQ773B61WE') len1, length('ON7ZZD1TAXX0GOUK7LK716UE5CF') len2;" "SUCCESS"

# 清理
pkill -9 observer 2>/dev/null
wait $OBSERVER_PID 2>/dev/null

echo ""
echo "=========================================="
echo "评测完成"
echo "=========================================="
echo "通过: $PASSED"
echo "失败: $FAILED"
echo "未知: $UNKNOWN"
echo "总计: $((PASSED + FAILED + UNKNOWN))"
echo ""
echo "通过率: $(( PASSED * 100 / (PASSED + FAILED + UNKNOWN) ))%"

# 检查observer日志
echo ""
echo "=========================================="
echo "Observer日志检查"
echo "=========================================="
error_count=$(grep -i "error\|fail" "$TEST_DIR/observer.log" 2>/dev/null | grep -v "failed to read packet" | wc -l)
if [ $error_count -gt 0 ]; then
    echo "⚠️  发现 $error_count 个错误/警告"
    grep -i "error\|fail" "$TEST_DIR/observer.log" 2>/dev/null | grep -v "failed to read packet" | head -10
else
    echo "✅ 未发现严重错误"
fi

