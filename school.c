#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define NUM_GRADES 12
#define NUM_CLASSES 10

// Structure to represent a student
typedef struct Student {
    char firstName[50];
    char lastName[50];
    char phoneNumber[15];
    int grade;
    int class;
    int marks[10];
    struct Student* next;
} Student;


// Function to create a new student node
Student* createStudentNode(const char* firstName, const char* lastName, const char* phoneNumber, int grade, int class, int marks[]) {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent != NULL) {
        strcpy(newStudent->firstName, firstName);
        strcpy(newStudent->lastName, lastName);
        strcpy(newStudent->phoneNumber, phoneNumber);
        newStudent->grade = grade;
        newStudent->class = class;
        for (int i = 0; i < 10; ++i) {
            newStudent->marks[i] = marks[i];
        }
        newStudent->next = NULL;
    }
    return newStudent;
}


// Function to add a student to a class
void addStudent(Student* school[NUM_GRADES][NUM_CLASSES], Student* newStudent) {
    if (school[newStudent->grade-1][newStudent->class-1] == NULL) {
        school[newStudent->grade-1][newStudent->class-1] = newStudent;
    } else {
        Student* currentStudent = school[newStudent->grade-1][newStudent->class-1];
        school[newStudent->grade-1][newStudent->class-1] = newStudent;
        newStudent->next = currentStudent;
    }
}


// Function to read students from a text file and add them to the school system
void readStudentsFromFile(const char* filename, Student* school[NUM_GRADES][NUM_CLASSES]) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open the file '%s'\n", filename);
        return;
    }

    char firstName[50], lastName[50], phoneNumber[15];
    int grade, classNum, marks[10];
    bool validStudent = true;

    while (fscanf(file, "%49s %49s %14s %d %d", firstName, lastName, phoneNumber, &grade, &classNum) == 5) {
        validStudent = true;

        if (grade < 1 || grade > NUM_GRADES || classNum < 1 || classNum > NUM_CLASSES) {
            printf("Error: Invalid grade or class number: %d %d\n", grade, classNum);
            validStudent = false;
        }

        for (int i = 0; i < 10; ++i) {
            if (fscanf(file, "%d", &marks[i]) != 1) {
                printf("Error: Failed to read marks for student %s %s\n", firstName, lastName);
                validStudent = false;
            }
        }

        // Clear the input buffer to consume any extra characters in the line
        int c;
        while ((c = fgetc(file)) != EOF && c != '\n');

        Student* newStudent = NULL;
        if (validStudent) {
            newStudent = createStudentNode(firstName, lastName, phoneNumber, grade, classNum, marks);
            if (newStudent == NULL) {
                printf("Error: Memory allocation failed.\n");
                fclose(file);
                return;
            }
            else {
                addStudent(school, newStudent);
            }
        }

    }

    fclose(file);
}



void printAllStudents(Student* school[NUM_GRADES][NUM_CLASSES]) {
    for (int grade = 0; grade < NUM_GRADES; ++grade) {
        for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
            Student* currentStudent = school[grade][classNum];
            while (currentStudent != NULL) {
                printf("Name: %s %s\n", currentStudent->firstName, currentStudent->lastName);
                printf("Grade: %d\n", currentStudent->grade);
                printf("Class: %d\n", currentStudent->class);
                printf("Phone Number: %s\n", currentStudent->phoneNumber);
                printf("Marks: ");
                for (int i = 0; i < 10; ++i) {
                    printf("%d ", currentStudent->marks[i]);
                }
                printf("\n\n");

                currentStudent = currentStudent->next;
            }
        }
    }
}


// Function to free the memory for each student node
void freeStudents(Student* school[NUM_GRADES][NUM_CLASSES]) {
    for (int grade = 0; grade < NUM_GRADES; ++grade) {
        for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
            Student* currentStudent = school[grade][classNum];
            while (currentStudent != NULL) {
                Student* temp = currentStudent;
                currentStudent = currentStudent->next;
                free(temp);
            }
        }
    }
}


int main() {
    Student* school[NUM_GRADES][NUM_CLASSES] = {0};

    const char* filename = "students_with_class.txt";
    readStudentsFromFile(filename, school);

    printAllStudents(school);

    freeStudents(school);
    return 0;
}