// student.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "student.h"

#define INITIAL_CAPACITY 4

// 从文本文件加载
void loadStudents(Student **students, int *count) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (!fp) {
        *students = NULL;
        *count = 0;
        return;
    }

    // 先统计行数（预分配内存）
    int lines = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') lines++;
    }
    // 处理最后一行无换行的情况
    if (lines > 0 || ftell(fp) > 0) {
        rewind(fp);
        if (fgetc(fp) != EOF) {
            rewind(fp);
            if (lines == 0) lines = 1;
        }
    } else {
        lines = 0;
    }
    rewind(fp);

    if (lines == 0) {
        *students = NULL;
        *count = 0;
        fclose(fp);
        return;
    }

    *students = (Student *)malloc(lines * sizeof(Student));
    if (!*students) {
        fprintf(stderr, "❌ 内存分配失败！\n");
        *count = 0;
        fclose(fp);
        return;
    }

    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), fp)) {
        // 去掉换行符
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;

        Student s;
        char nameBuf[NAME_LEN];
        int items = sscanf(line, "%d|%[^|]|%d|%f", &s.id, nameBuf, &s.age, &s.score);
        if (items == 4) {
            strncpy(s.name, nameBuf, NAME_LEN - 1);
            s.name[NAME_LEN - 1] = '\0';
            (*students)[i++] = s;
        }
        // 如果解析失败，跳过该行（容错）
    }

    *count = i;
    fclose(fp);
}

// 保存到文本文件（UTF-8）
void saveStudents(Student *students, int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (!fp) {
        printf("❌ 无法创建或写入 %s\n", DATA_FILE);
        return;
    }

    // 设置文件为 UTF-8（Windows 下确保 BOM 可选）
    // 对纯文本，通常不需要 BOM，但可加（见下方注释）
    // fprintf(fp, "\xEF\xBB\xBF"); // ← 取消注释可写入 UTF-8 BOM

    for (int i = 0; i < count; i++) {
        // 格式: id|name|age|score
        fprintf(fp, "%d|%s|%d|%.2f\n",
                students[i].id,
                students[i].name,
                students[i].age,
                students[i].score);
    }
    fclose(fp);
}

// --- 以下函数基本不变（仅调整参数类型）---

static Student* resizeArray(Student *arr,int newSize) {
    Student *newArr = realloc(arr, newSize * sizeof(Student));
    if (!newArr) {
        free(arr);
        exit(EXIT_FAILURE);
    }
    return newArr;
}

void addStudent(Student **students, int *count) {
    if (*count == 0) {
        *students = malloc(INITIAL_CAPACITY * sizeof(Student));
        if (!*students) { fprintf(stderr, "内存失败\n"); return; }
    } else if (*count % INITIAL_CAPACITY == 0) {
        *students = resizeArray(*students, (*count / INITIAL_CAPACITY + 1) * INITIAL_CAPACITY);
    }

    Student s;
    printf("请输入学号: ");
    scanf("%d", &s.id);
    while (getchar() != '\n');
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

void deleteStudent(Student **students, int *count) {
    if (*count == 0) { printf("暂无学生数据！\n"); return; }
    int id; printf("请输入要删除的学生学号: "); scanf("%d", &id);
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if ((*students)[i].id == id) {
            for (int j = i; j < *count - 1; j++) {
                (*students)[j] = (*students)[j + 1];
            }
            (*count)--;
            found = 1;
            printf("✅ 学号 %d 的学生已删除！\n", id);
            break;
        }
    }
    if (!found) printf("❌ 未找到学号为 %d 的学生！\n", id);
}

void searchStudent(Student *students, int count) {
    if (count == 0) { printf("暂无学生数据！\n"); return; }
    int id; printf("请输入要查询的学生学号: "); scanf("%d", &id);
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
    if (count == 0) { printf("暂无学生数据！\n"); return; }
    printf("\n=== 所有学生信息 ===\n");
    printf("%-8s %-10s %-6s %-8s\n", "学号", "姓名", "年龄", "成绩");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-10s %-6d %-8.2f\n",
               students[i].id, students[i].name,
               students[i].age, students[i].score);
    }
}

void freeStudents(Student *students) {
    free(students);
}

void showMenu() {
    system("cls");
    printf("========================================\n");
    printf("   学生信息管理系统 (文本文件版)\n");
    printf("========================================\n");
    printf("1. 添加学生\n2. 删除学生\n3. 查询学生\n4. 显示所有学生\n0. 退出\n");
    printf("========================================\n");
    printf("请选择 (0-4): ");
}