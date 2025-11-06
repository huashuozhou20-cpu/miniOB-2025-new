#!/bin/bash
./build/bin/observer -s /tmp/test_manual.sock -P mysql -t mvcc -d disk > /tmp/observer_test.log 2>&1 &
PID=$!
echo "Observer PID: $PID"

# 等待并检查日志
for i in {1..20}; do
    sleep 1
    if grep -q "Listen on unix socket" /tmp/observer_test.log 2>/dev/null; then
        echo "✅ Socket 已创建"
        ls -la /tmp/test_manual.sock 2>&1
        break
    fi
    if [ $i -eq 20 ]; then
        echo "❌ 超时，未找到 socket 创建消息"
        tail -50 /tmp/observer_test.log
    fi
done

pkill -9 observer 2>/dev/null || true
