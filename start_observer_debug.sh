#!/bin/bash
echo "=========================================="
echo "启动 Observer（调试模式）"
echo "=========================================="

# 清理
pkill -9 observer 2>/dev/null
sleep 2
rm -f observer.log

# 启动 observer（前台运行，可以看到所有输出）
echo "启动 observer..."
echo "命令: ./build/bin/observer -p 6789 -P mysql -t mvcc -d disk"
echo ""
echo "请等待看到 'Listen on port 6789' 或 'Observer start success' 消息"
echo "如果看到这些消息，说明网络服务已启动"
echo ""
echo "按 Ctrl+C 停止 observer"
echo "=========================================="
echo ""

./build/bin/observer -p 6789 -P mysql -t mvcc -d disk
