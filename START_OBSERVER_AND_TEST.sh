#!/bin/bash
echo "=========================================="
echo "启动 Observer 并测试 DELETE 语句"
echo "=========================================="

# 清理旧进程
pkill -9 observer 2>/dev/null
sleep 2

# 启动 observer
echo "启动 observer..."
./build/bin/observer -p 6789 -P mysql -t mvcc -d disk > observer.log 2>&1 &
OBSERVER_PID=$!
echo "Observer PID: $OBSERVER_PID"

# 等待启动
echo "等待 observer 启动..."
for i in {1..15}; do
    sleep 1
    if ss -tlnp 2>/dev/null | grep -q 6789 || netstat -tlnp 2>/dev/null | grep -q 6789; then
        echo "✅ 端口 6789 已监听 (等待了 ${i} 秒)"
        break
    fi
    if [ $i -eq 15 ]; then
        echo "⚠️  端口仍未监听，查看日志:"
        tail -20 observer.log | grep -E "Listen|start|Observer|ERROR|port" || tail -20 observer.log
        pkill -9 observer 2>/dev/null
        exit 1
    fi
done

sleep 2

# 测试 DELETE 语句
echo ""
echo "=========================================="
echo "测试 DELETE 语句"
echo "=========================================="

echo ""
echo "1. 创建表:"
echo "CREATE TABLE t_basic(id int, age int, name char(4), score float);" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn more" | head -3

echo ""
echo "2. 插入数据:"
echo "INSERT INTO t_basic VALUES(24, 20, 'test', 85.5);" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn more" | head -3

echo ""
echo "3. 查询数据 (删除前):"
echo "SELECT * FROM t_basic WHERE id=24;" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn more" | head -5

echo ""
echo "4. 执行 DELETE:"
echo "DELETE FROM t_basic WHERE id=24;" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn more" | head -3

echo ""
echo "5. 查询数据 (删除后):"
echo "SELECT * FROM t_basic WHERE id=24;" | ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn more" | head -5

echo ""
echo "=========================================="
echo "测试完成"
echo "=========================================="

# 清理
pkill -9 observer 2>/dev/null
