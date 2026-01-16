// student.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "sqlite3.h"
#include "student.h"

// 初始化数据库 & 创建表
int initDatabase() {
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "❌ 无法打开数据库: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    // 创建学生表（如果不存在）
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "age INTEGER NOT NULL,"
        "score REAL NOT NULL"
        ");";

    char *errMsg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "❌ SQL 错误: %s\n", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return rc;
    }

    sqlite3_close(db);
    return SQLITE_OK;
}

// 回调函数：用于查询结果
static int displayCallback(void *data, int argc, char **argv, char **azColName) {
    printf("%-8s %-10s %-6s %-8s\n",
           argv[0] ? argv[0] : "NULL",
           argv[1] ? argv[1] : "NULL",
           argv[2] ? argv[2] : "NULL",
           argv[3] ? argv[3] : "NULL");
    return 0;
}

// 添加学生
void addStudent() {
    sqlite3 *db;
    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        printf("❌ 无法连接数据库\n");
        return;
    }

    int id, age;
    float score;
    char name[NAME_LEN];

    printf("请输入学号: ");
    scanf("%d", &id);
    while (getchar() != '\n');
    printf("请输入姓名: ");
    fgets(name, NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0';
    printf("请输入年龄: ");
    scanf("%d", &age);
    printf("请输入成绩: ");
    scanf("%f", &score);

    // 使用参数化查询防止 SQL 注入
    const char *sql = "INSERT INTO students (id, name, age, score) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("❌ SQL 准备失败: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, age);
    sqlite3_bind_double(stmt, 4, (double)score);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("❌ 插入失败: %s\n", sqlite3_errmsg(db));
    } else {
        printf("✅ 学生信息添加成功！\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// 删除学生
void deleteStudent() {
    sqlite3 *db;
    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        printf("❌ 无法连接数据库\n");
        return;
    }

    int id;
    printf("请输入要删除的学生学号: ");
    scanf("%d", &id);

    const char *sql = "DELETE FROM students WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("❌ SQL 准备失败\n");
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    int changes = sqlite3_changes(db);
    if (changes > 0) {
        printf("✅ 学号 %d 的学生已删除！\n", id);
    } else {
        printf("❌ 未找到学号为 %d 的学生！\n", id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// 查询学生
void searchStudent() {
    sqlite3 *db;
    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        printf("❌ 无法连接数据库\n");
        return;
    }

    int id;
    printf("请输入要查询的学生学号: ");
    scanf("%d", &id);

    const char *sql = "SELECT id, name, age, score FROM students WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("❌ SQL 准备失败\n");
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("\n--- 查询结果 ---\n");
        printf("学号: %d\n", sqlite3_column_int(stmt, 0));
        printf("姓名: %s\n", sqlite3_column_text(stmt, 1));
        printf("年龄: %d\n", sqlite3_column_int(stmt, 2));
        printf("成绩: %.2f\n", sqlite3_column_double(stmt, 3));
    } else {
        printf("❌ 未找到学号为 %d 的学生！\n", id);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// 显示所有学生
void displayAllStudents() {
    sqlite3 *db;
    if (sqlite3_open(DB_FILE, &db) != SQLITE_OK) {
        printf("❌ 无法连接数据库\n");
        return;
    }

    const char *sql = "SELECT id, name, age, score FROM students;";
    char *errMsg = 0;

    printf("\n=== 所有学生信息 ===\n");
    printf("%-8s %-10s %-6s %-8s\n", "学号", "姓名", "年龄", "成绩");
    printf("----------------------------------------\n");

    int rc = sqlite3_exec(db, sql, displayCallback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        printf("❌ 查询失败: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
}

// 菜单
void showMenu() {
    system("cls");
    printf("========================================\n");
    printf("   学生信息管理系统 (SQLite 数据库版)\n");
    printf("========================================\n");
    printf("1. 添加学生\n2. 删除学生\n3. 查询学生\n4. 显示所有学生\n0. 退出\n");
    printf("========================================\n");
    printf("请选择 (0-4): ");
}