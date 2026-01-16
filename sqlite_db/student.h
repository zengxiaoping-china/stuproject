// student.h
#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define NAME_LEN 50
#define DB_FILE "students.db"

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    float score;
} Student;

// 函数声明（不再需要 load/save）
int initDatabase();
void addStudent();
void deleteStudent();
void searchStudent();
void displayAllStudents();
void showMenu();

#endif