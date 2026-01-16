// student.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "student.h"

const char DATA_FILE[] = "students.dat";

#define INITIAL_CAPACITY 4      // 初始容量
#define GROW_FACTOR 2           // 扩容倍数

// 从文件加载（动态分配内存）
void loadStudents(Student **students, int *count) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        *students = NULL;
        *count = 0;
        return;
    }

    fread(count, sizeof(int), 1, fp);
    if (*count <= 0) {
        *students = NULL;
        fclose(fp);
        return;
    }

    *students = (Student *)malloc(*count * sizeof(Student));
    if (!*students) {
        fprintf(stderr, "❌ 内存分配失败！\n");
        *count = 0;
        fclose(fp);
        return;
    }

    fread(*students, sizeof(Student), *count, fp);
    fclose(fp);
}

// 保存到文件（格式不变）
void saveStudents(Student *students, int count) {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (!fp) {
        printf("❌ 无法保存数据到 %s\n", DATA_FILE);
        return;
    }
    fwrite(&count, sizeof(int), 1, fp);
    if (count > 0) {
        fwrite(students, sizeof(Student), count, fp);
    }
    fclose(fp);
}

// 扩容函数（内部使用）
static Student* resizeArray(Student *arr,int newSize) {
    Student *newArr = (Student *)realloc(arr, newSize * sizeof(Student));
    if (!newArr) {
        fprintf(stderr, "❌ 内存扩容失败！\n");
        free(arr); // 防止内存泄漏
        exit(EXIT_FAILURE);
    }
    return newArr;
}

// 添加学生（自动扩容）
void addStudent(Student **students, int *count) {
    // 第一次添加？
    if (*count == 0) {
        *students = (Student *)malloc(INITIAL_CAPACITY * sizeof(Student));
        if (!*students) {
            fprintf(stderr, "❌ 初始内存分配失败！\n");
            return;
        }
    }
    // 检查是否需要扩容
    else if (*count % INITIAL_CAPACITY == 0) {
        int newCapacity = (*count / INITIAL_CAPACITY + 1) * INITIAL_CAPACITY;
        *students = resizeArray(*students, newCapacity);
    }

    Student s;
    printf("请输入学号: ");
    scanf("%d", &s.id);

    while (getchar() != '\n'); // 清缓冲区
    printf("请输入姓名: ");
    fgets(s.name, NAME_LEN, stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("请输入年龄: ");
    scanf("%d", &s.age);
    printf("请输入成绩: ");
    scanf("%f", &s.score);

    (*students)[*count] = s;
    (*count)++;
    printf("✅ 学生信息添加成功！\n");
}

// 删除学生（不缩容，保持简单）
void deleteStudent(Student **students, int *count) {
    if (*count == 0) {
        printf("暂无学生数据！\n");
        return;
    }

    int id;
    printf("请输入要删除的学生学号: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < *count; i++) {
        if ((*students)[i].id == id) {
            // 前移覆盖
            for (int j = i; j < *count - 1; j++) {
                (*students)[j] = (*students)[j + 1];
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

// 查询 & 显示（不变，只改参数类型）
void searchStudent(Student *students, int count) {
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

void displayAllStudents(Student *students, int count) {
    if (count == 0) {
        printf("暂无学生数据！\n");
        return;
    }
    printf("\n=== 所有学生信息 ===\n");
    printf("%-8s %-10s %-6s %-8s\n", "学号", "姓名", "年龄", "成绩");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-10s %-6d %-8.2f\n",
               students[i].id, students[i].name,
               students[i].age, students[i].score);
    }
}

// 释放内存
void freeStudents(Student *students) {
    if (students) {
        free(students);
    }
}

// 菜单（不变）
void showMenu() {
    system("cls");
    printf("========================================\n");
    printf("       学生信息管理系统 (动态内存版)\n");
    printf("========================================\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 查询学生\n");
    printf("4. 显示所有学生\n");
    printf("0. 退出系统\n");
    printf("========================================\n");
    printf("请选择操作 (0-4): ");
}