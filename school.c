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

enum menu_inputs {
    Insert = '0',
    Delete = '1',
    Edit = '2',
    Search = '3',
    Showall = '4',
    Top10 = '5',
    UnderperformedStudents = '6',
    Average = '7',
    Export = '8',
    Exit = '9'
};

enum courses {
    Biology = 0,
    Algebra = 1,
    Chemistry = 2,
    Calculus = 3,
    English = 4,
    History = 5,
    Physics = 6,
    Grammar = 7,
    Geography = 8,
    Statistics = 9
};

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


// Function to add a student to the school
void addStudent(Student* school[NUM_GRADES][NUM_CLASSES], Student* newStudent) {
    if (school[newStudent->grade-1][newStudent->class-1] == NULL) {
        // If no student in class - add as first student
        school[newStudent->grade-1][newStudent->class-1] = newStudent;
    } else {
        // Add student to the head of the list
        Student* currentStudent = school[newStudent->grade-1][newStudent->class-1];
        school[newStudent->grade-1][newStudent->class-1] = newStudent;
        newStudent->next = currentStudent;
    }
}


// Function to find a student by first name and last name in the school system
Student* findStudent(Student* school[NUM_GRADES][NUM_CLASSES], const char* firstName, const char* lastName) {
    for (int grade = 0; grade < NUM_GRADES; ++grade) {
        for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
            Student* currentStudent = school[grade][classNum];
            while (currentStudent != NULL) {
                if (strcmp(currentStudent->firstName, firstName) == 0 &&
                    strcmp(currentStudent->lastName, lastName) == 0) {
                    return currentStudent;
                }
                currentStudent = currentStudent->next;
            }
        }
    }
    return NULL; // Student not found
}


// Function to delete a student from the school system
void deleteStudent(Student* school[NUM_GRADES][NUM_CLASSES]) {
    char firstName[50], lastName[50];

    printf("Enter the first name of the student to delete: ");
    scanf("%49s", firstName);

    printf("Enter the last name of the student to delete: ");
    scanf("%49s", lastName);

    // Find the student in the school system
    Student* targetStudent = findStudent(school, firstName, lastName);

    if (targetStudent == NULL) {
        printf("Student not found.\n");
        return;
    }

    printf("Found student:\n");
    printf("Name: %s %s\n", targetStudent->firstName, targetStudent->lastName);
    printf("Grade: %d\n", targetStudent->grade);
    printf("Class: %d\n", targetStudent->class);
    printf("Phone Number: %s\n", targetStudent->phoneNumber);
    printf("\n");

    // Ask the user to confirm the deletion
    printf("Do you want to delete this student? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        // Delete the student
        for (int grade = 0; grade < NUM_GRADES; ++grade) {
            for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
                Student* currentStudent = school[grade][classNum];
                Student* prevStudent = NULL;
                while (currentStudent != NULL) {
                    if (currentStudent == targetStudent) {
                        if (prevStudent != NULL) {
                            prevStudent->next = currentStudent->next;
                        } else {
                            school[grade][classNum] = currentStudent->next;
                        }
                        free(targetStudent);
                        printf("Student deleted successfully.\n");
                        return;
                    }
                    prevStudent = currentStudent;
                    currentStudent = currentStudent->next;
                }
            }
        }
    } else {
        printf("Deletion cancelled.\n");
    }
}


// Function to get new student details from the user and perform validation
void addNewStudent(Student* school[NUM_GRADES][NUM_CLASSES]) {
    char firstName[50], lastName[50], phoneNumber[15];
    int grade, classNum, marks[10];

    printf("Enter the first name: ");
    scanf("%49s", firstName);
    printf("Enter the last name: ");
    scanf("%49s", lastName);
    printf("Enter the phone number: ");
    scanf("%14s", phoneNumber);
    printf("Enter the grade (1-12): ");
    scanf("%d", &grade);
    printf("Enter the class number (1-10): ");
    scanf("%d", &classNum);

    if (grade < 1 || grade > NUM_GRADES || classNum < 1 || classNum > NUM_CLASSES) {
        printf("Error: Invalid grade or class number.\n");
        return;
    }

    printf("Enter the marks for 10 subjects (separated by spaces): ");
    for (int i = 0; i < 10; ++i) {
        scanf("%d", &marks[i]);
    }

    // Clear the input buffer to consume any extra characters in the line
    int c;
    while ((c = getchar()) != EOF && c != '\n');

    Student* newStudent = createStudentNode(firstName, lastName, phoneNumber, grade, classNum, marks);
    if (newStudent == NULL) {
        printf("Error: Memory allocation failed.\n");
    } else {
        addStudent(school, newStudent);
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


// Function to print all the students in the school
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

// Function to search for a student in the school system
void searchStudent(Student* school[NUM_GRADES][NUM_CLASSES]) {
    char firstName[50], lastName[50];

    printf("Enter the first name of the student to search: ");
    scanf("%49s", firstName);

    printf("Enter the last name of the student to search: ");
    scanf("%49s", lastName);

    // Find the student in the school system
    Student* targetStudent = findStudent(school, firstName, lastName);

    if (targetStudent == NULL) {
        printf("Student not found.\n");
    } else {
        printf("Found student:\n");
        printf("Name: %s %s\n", targetStudent->firstName, targetStudent->lastName);
        printf("Grade: %d\n", targetStudent->grade);
        printf("Class: %d\n", targetStudent->class);
        printf("Phone Number: %s\n", targetStudent->phoneNumber);
        printf("Marks: ");
        for (int i = 0; i < 10; ++i) {
            printf("%d ", targetStudent->marks[i]);
        }
        printf("\n");
    }
}

void menu(Student* school[NUM_GRADES][NUM_CLASSES]) {
    char input;
    // school.name = "schoolName";
    do {
        printf("\n|School Manager<::>Home|\n");
        printf("--------------------------------------------------------------------------------\n");
        //printf("Welcome to ( %s ) School!\nYou have inserted ( %zu ) students.\n\n", school.name, school.numOfStudents);
        printf("\t[0] |--> Insert\n");
        printf("\t[1] |--> Delete\n");
        printf("\t[2] |--> Edit\n");
        printf("\t[3] |--> Search\n");
        printf("\t[4] |--> Show All\n");
        printf("\t[5] |--> Top 10 students per course\n");
        printf("\t[6] |--> Underperformed students\n");
        printf("\t[7] |--> Average per course\n");
        printf("\t[8] |--> Export\n");
        printf("\t[9] |--> Exit\n");
        printf("\n\tPlease Enter Your Choice (0-9): ");
        input = getc(stdin);
        fflush(stdin);
        getc(stdin);
        switch (input) {
            case Insert:
                addNewStudent(school);
                break;
            case Delete:
                deleteStudent(school);
                break;
            case Edit:
                //editStudentGrade();
                break;
            case Search:
                searchStudent(school);
                break;
            case Showall:
                //printAllStudents();
                break;
            case Top10:
                //printTopNStudentsPerCourse();
                break;
            case UnderperformedStudents:
                //printUnderperformedStudents();
                break;
            case Average:
                //printAverage();
                break;
            case Export:
                //exportDatabase();
                break;
            case Exit:
                //handleClosing();
                break;
            default:
                printf("\nThere is no item with symbol \"%c\".Please enter a number between 1-10!\nPress any key to continue...",
                       input);
                getc(stdin);
                getc(stdin);
                break;
        }
    } while (input != Exit);
}


int main() {
    Student* school[NUM_GRADES][NUM_CLASSES] = {0};

    const char* filename = "students_with_class.txt";
    readStudentsFromFile(filename, school);

    // printAllStudents(school);
    menu(school);


    freeStudents(school);
    return 0;
}