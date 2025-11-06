#!/bin/bash
echo "=========================================="
echo "启动 Observer（修复版 - 使用低内存模式）"
echo "=========================================="

# 清理
pkill -9 observer 2>/dev/null
sleep 2

# 使用较小的 buffer pool (8MB) 避免内存问题
echo "使用较小的 buffer pool (8MB) 以避免内存问题..."
echo "命令: ./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608"
echo ""
echo "等待看到 'Listen on port 6789' 或 'Observer start success' 消息"
echo "按 Ctrl+C 停止 observer"
echo "=========================================="
echo ""

./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608
