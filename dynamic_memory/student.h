// student.h

#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define NAME_LEN 50

typedef struct{
    int id;
    char name[NAME_LEN];
    int age;
    float score;
}Student;

// 注意：现在 students 是指针，count 是当前数量
void loadStudents(Student **students, int *count);
void saveStudents(Student *students, int count);
void addStudent(Student **students, int *count);
void deleteStudent(Student **students, int *count);
void searchStudent(Student *students, int count);
void displayAllStudents(Student *students, int count);
void showMenu();
void freeStudents(Student *students); // 新增：释放内存

#endif