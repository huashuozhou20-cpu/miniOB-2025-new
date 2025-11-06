#!/bin/bash
echo "检查 Observer 状态..."
echo ""

# 检查进程
echo "1. 进程状态:"
ps aux | grep "[o]bserver" | head -3 || echo "  无 observer 进程"

# 检查端口
echo ""
echo "2. 端口监听状态:"
ss -tlnp 2>/dev/null | grep 6789 || netstat -tlnp 2>/dev/null | grep 6789 || echo "  端口 6789 未监听"

# 检查日志
echo ""
echo "3. 日志文件状态:"
if [ -f observer.log ]; then
    echo "  日志文件存在，行数: $(wc -l < observer.log)"
    echo "  最后10行:"
    tail -10 observer.log | sed 's/^/    /'
else
    echo "  日志文件不存在"
fi

# 尝试连接测试
echo ""
echo "4. 连接测试:"
if ss -tlnp 2>/dev/null | grep -q ":6789"; then
    echo "  端口已监听，尝试连接..."
    echo "SELECT 1;" | timeout 3 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn" | head -3 || echo "  连接失败"
else
    echo "  端口未监听，无法测试连接"
fi
