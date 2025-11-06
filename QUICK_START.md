# 快速启动指南

## 问题总结
Observer 启动时被 "Killed"，原因是默认 buffer pool 太大。

## 解决方案

### ✅ 推荐方法：使用低内存模式

**终端1 - 启动 observer：**
```bash
cd /home/obuser/oceanbase-contest/test/miniob-2025-new
./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 8388608
```

**等待看到以下消息之一：**
- "Listen on port 6789"
- "Observer start success"

**终端2 - 测试连接：**
```bash
cd /home/obuser/oceanbase-contest/test/miniob-2025-new
./build/bin/obclient -h 127.0.0.1 -p 6789
```

**测试 DELETE 语句：**
```sql
CREATE TABLE t_basic(id int, age int, name char(4), score float);
INSERT INTO t_basic VALUES(24, 20, 'test', 85.5);
SELECT * FROM t_basic WHERE id=24;
DELETE FROM t_basic WHERE id=24;
SELECT * FROM t_basic WHERE id=24;
```

## 如果仍然被 Killed

1. **检查系统内存：**
   ```bash
   free -h
   ```

2. **检查系统日志：**
   ```bash
   dmesg | tail -20
   ```

3. **尝试更小的 buffer pool（4MB）：**
   ```bash
   ./build/bin/observer -p 6789 -P mysql -t mvcc -d disk -n 4194304
   ```

## 已创建的脚本

- `start_observer_fixed.sh` - 使用低内存模式启动
- `test_connection.sh` - 测试客户端连接
- `FINAL_START_GUIDE.md` - 详细启动指南
