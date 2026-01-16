// student.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

const char DATA_FILE[] = "students.dat";

// 从文件加载学生数据
void loadStudents(Student students[], int *count) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        *count = 0;
        return;
    }
    fread(count, sizeof(int), 1, fp);
    if (*count > 0 && *count <= MAX_STUDENTS) {
        fread(students, sizeof(Student), *count, fp);
    } else {
        *count = 0;
    }
    fclose(fp);
}

// 保存学生数据到文件
void saveStudents(Student students[], int count) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("警告：无法保存数据到文件！\n");
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    if (count > 0) {
        fwrite(students, sizeof(Student), count, fp);
    }
    fclose(fp);
}

// 添加学生
void addStudent(Student students[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("错误：学生数量已达上限（%d）！\n", MAX_STUDENTS);
        return;
    }

    Student s;
    printf("请输入学号: ");
    scanf("%d", &s.id);

    // 清空输入缓冲区（非常重要！）
    while (getchar() != '\n');

    printf("请输入姓名: ");
    fgets(s.name, NAME_LEN, stdin);
    // 去掉 fgets 读入的换行符 \n
    size_t len = strlen(s.name);
    if (len > 0 && s.name[len - 1] == '\n') {
        s.name[len - 1] = '\0';
    }

    printf("请输入年龄: ");
    scanf("%d", &s.age);
    printf("请输入成绩: ");
    scanf("%f", &s.score);

    students[*count] = s;
    (*count)++;
    printf("✅ 学生信息添加成功！\n");
}

// 删除学生（按学号）
void deleteStudent(Student students[], int *count) {
    if (*count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    int id;
    printf("请输入要删除的学生学号: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < *count; i++) {
        if (students[i].id == id) {
            // 前移覆盖
            for (int j = i; j < *count - 1; j++) {
                students[j] = students[j + 1];
            }
            (*count)--;
            found = 1;
            printf("✅ 学号 %d 的学生已删除！\n", id);
            break;
        }
    }
    if (!found) {
        printf("❌ 未找到学号为 %d 的学生！\n", id);
    }
}

// 查找学生（按学号）
void searchStudent(Student students[], int count) {
    if (count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    int id;
    printf("请输入要查询的学生学号: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            printf("\n--- 查询结果 ---\n");
            printf("学号: %d\n", students[i].id);
            printf("姓名: %s\n", students[i].name);
            printf("年龄: %d\n", students[i].age);
            printf("成绩: %.2f\n", students[i].score);
            return;
        }
    }
    printf("❌ 未找到学号为 %d 的学生！\n", id);
}

// 显示所有学生
void displayAllStudents(Student students[], int count) {
    if (count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    printf("\n=== 所有学生信息 ===\n");
    printf("%-8s %-10s %-6s %-8s\n", "学号", "姓名", "年龄", "成绩");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-10s %-6d %-8.2f\n",
               students[i].id,
               students[i].name,
               students[i].age,
               students[i].score);
    }
}

// 显示菜单
void showMenu() {
    system("cls"); // Windows 清屏；Linux/macOS 用 system("clear");
    printf("========================================\n");
    printf("       学生信息管理系统 (C语言版)\n");
    printf("========================================\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 查询学生\n");
    printf("4. 显示所有学生\n");
    printf("0. 退出系统\n");
    printf("========================================\n");
    printf("请选择操作 (0-4): ");
}