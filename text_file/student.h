// student.h
#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define NAME_LEN 50
#define DATA_FILE "students.txt"  // ← 改为文本文件！

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    float score;
} Student;

// 函数声明（接口不变）
void loadStudents(Student **students, int *count);
void saveStudents(Student *students, int count);
void addStudent(Student **students, int *count);
void deleteStudent(Student **students, int *count);
void searchStudent(Student *students, int count);
void displayAllStudents(Student *students, int count);
void showMenu();
void freeStudents(Student *students);

#endif