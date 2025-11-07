#!/bin/bash
# 全面功能测试脚本 - 改进版

TEST_DIR="/tmp/miniob_test_manual"
cd /home/obuser/oceanbase-contest/test/miniob-2025-new

# 测试函数 - 每次测试都启动新的observer
test_sql() {
    local test_name="$1"
    local sql="$2"
    
    echo ""
    echo "============================================================"
    echo "测试: $test_name"
    echo "============================================================"
    echo "SQL: $sql"
    
    # 清理
    rm -rf $TEST_DIR
    mkdir -p $TEST_DIR
    
    # 启动observer
    timeout 10 ./build/bin/observer -s $TEST_DIR/miniob.sock -P mysql -t mvcc -d disk > $TEST_DIR/observer.log 2>&1 &
    OBSERVER_PID=$!
    sleep 3
    
    # 创建临时SQL文件
    echo "$sql" > $TEST_DIR/test.sql
    
    # 执行SQL
    timeout 5 ./build/bin/obclient -s $TEST_DIR/miniob.sock < $TEST_DIR/test.sql 2>&1 | \
        grep -v "Welcome\|Copyright\|Learn\|miniob >" | \
        head -30
    
    # 停止observer
    kill $OBSERVER_PID 2>/dev/null
    wait $OBSERVER_PID 2>/dev/null
    sleep 1
    
    rm -f $TEST_DIR/test.sql
}

echo "开始全面功能测试..."
    echo ""

# 测试1: NULL/NOT NULL
test_sql "1. NULL/NOT NULL" "CREATE TABLE test_null(id int, name char(10) NULL, age int NOT NULL);
INSERT INTO test_null VALUES(1, NULL, 20);
SELECT * FROM test_null;"

# 测试2: 唯一索引
test_sql "2. 唯一索引" "CREATE TABLE test_unique(id int, name char(10));
INSERT INTO test_unique VALUES(1, 'a');
CREATE UNIQUE INDEX idx_id ON test_unique(id);
INSERT INTO test_unique VALUES(1, 'b');"

# 测试3: 系统函数
test_sql "3. 系统函数" "CREATE TABLE test_func(id int, name char(10), price float, date_col date);
INSERT INTO test_func VALUES(1, 'test', 12.345, '2020-01-21');
SELECT LENGTH(name) FROM test_func;
SELECT ROUND(price) FROM test_func;
SELECT DATE_FORMAT(date_col, '%Y-%m-%d') FROM test_func;"

# 测试4: UNION
test_sql "4. UNION" "CREATE TABLE test_union1(id int);
CREATE TABLE test_union2(id int);
INSERT INTO test_union1 VALUES(1);
INSERT INTO test_union1 VALUES(2);
INSERT INTO test_union2 VALUES(2);
INSERT INTO test_union2 VALUES(3);
SELECT * FROM test_union1 UNION SELECT * FROM test_union2;
SELECT * FROM test_union1 UNION ALL SELECT * FROM test_union2;"

# 测试5: JOIN
test_sql "5. JOIN" "CREATE TABLE test_join1(id int, name char(10));
CREATE TABLE test_join2(id int, num int);
INSERT INTO test_join1 VALUES(1, 'a');
INSERT INTO test_join1 VALUES(2, 'b');
INSERT INTO test_join2 VALUES(1, 10);
INSERT INTO test_join2 VALUES(2, 20);
SELECT * FROM test_join1 INNER JOIN test_join2 ON test_join1.id=test_join2.id;"

# 测试6: Vector类型和STRING_TO_VECTOR
test_sql "6. Vector类型和STRING_TO_VECTOR" "CREATE TABLE test_vector(id int, vec vector(3));
INSERT INTO test_vector VALUES(1, STRING_TO_VECTOR('[1.0,2.0,3.0]'));
SELECT * FROM test_vector;"

# 测试7: ALTER TABLE
test_sql "7. ALTER TABLE" "CREATE TABLE test_alter(id int, name char(10));
ALTER TABLE test_alter ADD COLUMN age int;
DESC test_alter;"

# 测试8: 日期类型索引
test_sql "8. 日期类型索引" "CREATE TABLE test_date(id int, date_col date);
CREATE INDEX idx_date ON test_date(date_col);
INSERT INTO test_date VALUES(1, '2020-01-21');
SELECT * FROM test_date WHERE date_col='2020-01-21';"

# 测试9: TOKENIZE
test_sql "9. TOKENIZE函数" "CREATE TABLE test_tokenize(id int, text char(100));
INSERT INTO test_tokenize VALUES(1, 'hello world');
SELECT TOKENIZE(text, 'jieba') FROM test_tokenize;"

    echo ""
echo "============================================================"
echo "所有测试完成！"
echo "============================================================"
