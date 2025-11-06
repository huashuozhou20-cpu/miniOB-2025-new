#!/bin/bash
killall -9 observer 2>/dev/null
pkill -9 -f observer 2>/dev/null
sleep 2

echo "启动 observer..."
./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608 2>&1 | tee /tmp/observer_run.log &
OBSERVER_PID=$!
echo "Observer PID: $OBSERVER_PID"

echo "等待10秒..."
sleep 10

echo ""
echo "查看输出（查找 DEBUG 信息）:"
grep -E "\[DEBUG\]|\[ERROR\]|\[PANIC\]|Initialization|Server|serve|thread|handler|network|start|listen|port|Observer start|Successfully init" /tmp/observer_run.log 2>/dev/null | head -20

echo ""
echo "检查端口:"
ss -tlnp 2>/dev/null | grep 6789 || echo "端口未监听"

if ss -tlnp 2>/dev/null | grep -q ":6789"; then
    echo ""
    echo "✅ 端口已监听！测试 DELETE..."
    echo "DELETE FROM t_basic WHERE id=24;" | timeout 3 ./build/bin/obclient -h 127.0.0.1 -p 6789 2>&1 | grep -v "Welcome\|Copyright\|Learn" | head -3
fi

pkill -9 observer 2>/dev/null
