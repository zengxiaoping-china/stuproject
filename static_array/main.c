// main.c
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "student.h"

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    Student students[MAX_STUDENTS];
    int count = 0;

    // 启动时加载数据
    loadStudents(students, &count);
    printf("✅ 已加载 %d 条学生记录。\n", count);
    getchar(); // 消费回车
    printf("按回车继续...");
    getchar();

    int choice;
    while (1) {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(students, &count);
                break;
            case 2:
                deleteStudent(students, &count);
                break;
            case 3:
                searchStudent(students, count);
                break;
            case 4:
                displayAllStudents(students, count);
                break;
            case 0:
                // 退出前保存数据
                saveStudents(students, count);
                printf("💾 数据已保存，感谢使用！再见！\n");
                return 0;
            default:
                printf("❌ 无效选项，请重新输入！\n");
        }
        printf("\n按回车继续...");
        while (getchar() != '\n'); // 清空输入缓冲区
        getchar();
    }

    return 0;
}