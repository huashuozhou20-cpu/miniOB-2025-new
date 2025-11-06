#!/bin/bash
echo "=========================================="
echo "安全启动 Observer"
echo "=========================================="

# 清理
pkill -9 observer 2>/dev/null
sleep 2

# 检查二进制文件
if [ ! -f ./build/bin/observer ]; then
    echo "❌ observer 二进制文件不存在"
    echo "请先编译: make -j4"
    exit 1
fi

# 检查二进制文件是否可执行
if [ ! -x ./build/bin/observer ]; then
    echo "❌ observer 二进制文件不可执行"
    exit 1
fi

# 设置资源限制（避免 OOM）
ulimit -v 2097152  # 2GB 虚拟内存限制
ulimit -m 1048576  # 1GB 物理内存限制

echo "资源限制:"
ulimit -a | grep -E "virtual|memory|file"

echo ""
echo "启动 observer..."
echo "命令: ./build/bin/observer -p 6789 -P mysql -t mvcc -d disk"
echo ""
echo "如果看到 'Killed'，可能是内存不足"
echo "=========================================="
echo ""

# 前台启动
./build/bin/observer -p 6789 -P mysql -t mvcc -d disk 2>&1
EXIT_CODE=$?

if [ $EXIT_CODE -eq 137 ] || [ $EXIT_CODE -eq 9 ]; then
    echo ""
    echo "⚠️  Observer 被系统终止 (退出码: $EXIT_CODE)"
    echo "可能原因: 内存不足 (OOM)"
    echo ""
    echo "建议:"
    echo "1. 检查系统内存: free -h"
    echo "2. 检查系统日志: dmesg | tail -20"
    echo "3. 尝试减少 buffer pool 大小: -n 8388608 (8MB)"
fi
