#!/bin/bash

echo "=========================================="
echo "Observer 启动问题诊断脚本"
echo "=========================================="

# 检查端口占用
echo ""
echo "1. 检查端口 6789 是否被占用:"
if lsof -i :6789 2>/dev/null | grep -q LISTEN; then
    echo "   ❌ 端口 6789 已被占用:"
    lsof -i :6789
else
    echo "   ✅ 端口 6789 未被占用"
fi

# 检查是否有 observer 进程
echo ""
echo "2. 检查是否有 observer 进程:"
OBSERVER_PIDS=$(pgrep -f observer | head -5)
if [ -n "$OBSERVER_PIDS" ]; then
    echo "   ⚠️  发现 observer 进程:"
    ps aux | grep observer | grep -v grep | head -5
    echo ""
    echo "   进程状态:"
    for pid in $OBSERVER_PIDS; do
        STATE=$(ps -o stat= -p $pid 2>/dev/null)
        echo "   PID $pid: $STATE"
    done
else
    echo "   ✅ 没有运行中的 observer 进程"
fi

# 检查系统资源
echo ""
echo "3. 检查系统资源:"
echo "   内存使用:"
free -h | head -2
echo ""
echo "   文件描述符限制:"
ulimit -n

# 检查日志文件
echo ""
echo "4. 检查最近的日志文件:"
if [ -f "observer.log" ]; then
    echo "   最近的 observer.log 内容:"
    tail -30 observer.log | grep -E "ERROR|PANIC|Listen|start|port|socket" || tail -30 observer.log
else
    echo "   ⚠️  未找到 observer.log 文件"
fi

# 尝试启动 observer 并监控
echo ""
echo "5. 尝试启动 observer (5秒超时):"
pkill -9 observer 2>/dev/null
sleep 1

./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608 > /tmp/observer_test.log 2>&1 &
OBSERVER_PID=$!
echo "   Observer PID: $OBSERVER_PID"

# 等待并检查
for i in {1..10}; do
    sleep 0.5
    if ss -tlnp 2>/dev/null | grep -q ":6789"; then
        echo "   ✅ 端口 6789 已监听 (等待了 $((i*500))ms)"
        break
    fi
    if ! kill -0 $OBSERVER_PID 2>/dev/null; then
        echo "   ❌ Observer 进程已退出"
        echo "   退出码: $(wait $OBSERVER_PID 2>/dev/null; echo $?)"
        echo ""
        echo "   Observer 输出:"
        cat /tmp/observer_test.log | tail -50
        break
    fi
done

if ss -tlnp 2>/dev/null | grep -q ":6789"; then
    echo ""
    echo "6. 测试连接:"
    echo "SELECT 1;" | timeout 2 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | head -5 || echo "   连接失败"
fi

# 清理
pkill -9 observer 2>/dev/null

echo ""
echo "=========================================="
echo "诊断完成"
echo "=========================================="

