#!/bin/bash
echo "=========================================="
echo "低内存模式启动 Observer"
echo "=========================================="

# 清理
pkill -9 observer 2>/dev/null
sleep 2

# 设置较小的 buffer pool（8MB 而不是默认的 16MB）
echo "使用较小的 buffer pool (8MB)..."
echo "命令: ./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608"
echo ""
echo "=========================================="
echo ""

./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608 2>&1
