// main.c
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    Student *students = NULL;
    int count = 0;

    loadStudents(&students, &count);
    printf("✅ 已加载 %d 条学生记录。\n", count);
    printf("按回车继续...");
    getchar();

    int choice;
    while (1) {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(&students, &count); break;
            case 2: deleteStudent(&students, &count); break;
            case 3: searchStudent(students, count); break;
            case 4: displayAllStudents(students, count); break;
            case 0:
                saveStudents(students, count);
                freeStudents(students); // 释放内存
                printf("💾 数据已保存，感谢使用！再见！\n");
                return 0;
            default:
                printf("❌ 无效选项，请重新输入！\n");
        }
        printf("\n按回车继续...");
        while (getchar() != '\n');
        getchar();
    }

    return 0;
}