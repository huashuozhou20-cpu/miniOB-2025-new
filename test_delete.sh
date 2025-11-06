#!/bin/bash
echo "=========================================="
echo "测试 DELETE 语句"
echo "=========================================="

echo ""
echo "1. 创建表:"
echo "CREATE TABLE t_basic(id int, age int, name char(4), score float);" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1

echo ""
echo "2. 插入数据:"
echo "INSERT INTO t_basic VALUES(24, 20, 'test', 85.5);" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1

echo ""
echo "3. 查询数据 (删除前):"
echo "SELECT * FROM t_basic WHERE id=24;" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1

echo ""
echo "4. 执行 DELETE:"
echo "DELETE FROM t_basic WHERE id=24;" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1

echo ""
echo "5. 查询数据 (删除后):"
echo "SELECT * FROM t_basic WHERE id=24;" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1

echo ""
echo "=========================================="
