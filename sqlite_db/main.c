// main.c
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    // 初始化数据库
    if (initDatabase() != 0) {
        printf("❌ 数据库初始化失败，程序退出。\n");
        return 1;
    }

    printf("✅ 数据库已准备就绪。\n");
    printf("按回车继续...");
    getchar();

    int choice;
    while (1) {
        showMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: searchStudent(); break;
            case 4: displayAllStudents(); break;
            case 0:
                printf("👋 感谢使用！再见！\n");
                return 0;
            default:
                printf("❌ 无效选项！\n");
        }
        printf("\n按回车继续...");
        while (getchar() != '\n'); getchar();
    }
}