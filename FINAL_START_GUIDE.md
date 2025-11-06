# Observer 启动问题解决方案

## 问题
Observer 启动时被 "Killed"，无法连接客户端。

## 原因
默认的 buffer pool 大小（16MB）可能导致内存不足，特别是在 WSL2 环境中。

## 解决方案

### 方法1: 使用低内存模式启动（推荐）

```bash
./start_observer_fixed.sh
```

或者直接运行：
```bash
./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608
```

参数说明：
- `-n 8388608`: 设置 buffer pool 为 8MB（8388608 字节）

### 方法2: 后台启动并测试

```bash
# 启动 observer
./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608 > observer.log 2>&1 &

# 等待几秒后测试连接
sleep 5
./test_connection.sh
```

## 测试 DELETE 语句

当 observer 启动成功后（看到 "Listen on port 6789"），在另一个终端运行：

```bash
./build/bin/obclient -h 127.0.0.1 -p 6789
```

然后执行：
```sql
CREATE TABLE t_basic(id int, age int, name char(4), score float);
INSERT INTO t_basic VALUES(24, 20, 'test', 85.5);
SELECT * FROM t_basic WHERE id=24;
DELETE FROM t_basic WHERE id=24;
SELECT * FROM t_basic WHERE id=24;
```

## 验证

检查端口是否监听：
```bash
ss -tlnp | grep 6789
```

如果看到端口监听，说明启动成功！
