#!/bin/bash
echo "=========================================="
echo "测试客户端连接"
echo "=========================================="

# 检查端口是否监听
if ss -tlnp 2>/dev/null | grep -q ":6789" || netstat -tlnp 2>/dev/null | grep -q ":6789"; then
    echo "✅ 端口 6789 正在监听"
    echo ""
    echo "测试连接和 DELETE 语句:"
    echo ""
    
    echo "1. 创建表:"
    echo "CREATE TABLE t_basic(id int, age int, name char(4), score float);" | timeout 5 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn" | head -3
    
    echo ""
    echo "2. 插入数据:"
    echo "INSERT INTO t_basic VALUES(24, 20, 'test', 85.5);" | timeout 5 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn" | head -3
    
    echo ""
    echo "3. 查询数据 (删除前):"
    echo "SELECT * FROM t_basic WHERE id=24;" | timeout 5 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn" | head -5
    
    echo ""
    echo "4. 执行 DELETE:"
    echo "DELETE FROM t_basic WHERE id=24;" | timeout 5 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn" | head -3
    
    echo ""
    echo "5. 查询数据 (删除后):"
    echo "SELECT * FROM t_basic WHERE id=24;" | timeout 5 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn" | head -5
    
    echo ""
    echo "=========================================="
else
    echo "❌ 端口 6789 未监听"
    echo ""
    echo "请先启动 observer:"
    echo "  ./start_observer_debug.sh"
    echo ""
    echo "或者后台启动:"
    echo "  ./build/bin/observer -p 6789 -P mysql -t mvcc -d disk > observer.log 2>&1 &"
    echo "  然后等待几秒，运行此脚本再次测试"
fi
