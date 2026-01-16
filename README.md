# 学生信息管理系统 — 四代演进版

本项目展示了从简单到专业的四种实现方式：

| 版本                |      名称       |               特点     |
|---------------------|----------------|------------------------|
| `v1_static_array`   | 静态数组版      | 固定容量，二进制文件存储 |
| `v2_dynamic_memory` | 动态内存版      | 无容量限制，自动扩容     |
| `v3_text_file`      | 文本文件版      | 人类可读，UTF-8 支持    |
| `v4_sqlite_db`      | SQLite 数据库版 | 专业级存储，支持复杂查询 |

## 编译说明
- **v1-v3**: `gcc main.c student.c -o student_system.exe`
- **v4**: `gcc main.c student.c sqlite3.c -o student_system.exe`

> 所有版本均支持中文姓名，需在 Windows 控制台运行。