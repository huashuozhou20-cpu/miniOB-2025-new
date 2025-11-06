# 客户端连接问题排查指南

## 问题现象
客户端无法连接到 observer（端口 6789 未监听）

## 排查步骤

### 1. 检查 observer 是否正在运行
```bash
ps aux | grep observer | grep -v grep
```

### 2. 检查端口是否监听
```bash
ss -tlnp | grep 6789
# 或
netstat -tlnp | grep 6789
```

### 3. 查看日志文件
```bash
tail -50 observer.log
```

查找关键信息：
- "Listen on port 6789" - 表示网络服务已启动
- "Observer start success" - 表示启动成功
- "ERROR" - 查看错误信息

### 4. 前台启动 observer（推荐）
```bash
./build/bin/observer -p 6789 -P mysql -t mvcc -d disk
```

前台启动可以看到完整的启动日志，包括：
- 初始化过程
- 网络服务启动信息
- 任何错误信息

### 5. 测试连接
在另一个终端运行：
```bash
./build/bin/obclient -h 127.0.0.1 -p 6789
```

或者使用测试脚本：
```bash
./test_connection.sh
```

## 常见问题

### 问题1: 端口未监听
**原因**: observer 的网络服务可能没有成功启动
**解决**: 
1. 前台启动 observer 查看完整日志
2. 检查是否有错误信息
3. 确认配置文件正确

### 问题2: 连接被拒绝
**原因**: observer 未启动或端口未监听
**解决**: 按照上述步骤启动 observer

### 问题3: 日志中没有 "Listen on port"
**原因**: 可能卡在初始化阶段
**解决**: 
1. 检查日志文件中的错误信息
2. 尝试前台启动查看完整输出
3. 检查系统资源（内存、文件描述符等）

## 快速测试脚本

已创建以下脚本：
- `start_observer_debug.sh` - 前台启动 observer（推荐）
- `test_connection.sh` - 测试客户端连接和 DELETE 语句

使用方法：
```bash
# 终端1: 启动 observer
./start_observer_debug.sh

# 终端2: 测试连接
./test_connection.sh
```
