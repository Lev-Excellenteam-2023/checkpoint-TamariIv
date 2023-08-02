#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define NUM_GRADES 12
#define NUM_CLASSES 10
#define MIN_AVERAGE 60

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

enum subjects {
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

// Array to map subject numbers to subject names
const char* subjectNames[] = {
    "Biology",
    "Algebra",
    "Chemistry",
    "Calculus",
    "English",
    "History",
    "Physics",
    "Grammar",
    "Geography",
    "Statistics"
};

Student* school[NUM_GRADES][NUM_CLASSES] = {0};

/*
 * The function gets input from the user and ensures the input
 * doesn't exceed max_size. if yes - it will shrink it and will add \0.
 * credit: Eitan Abraham
 */
size_t get_input(char* dst, size_t max_size) {
    char* input = NULL;
    size_t len = 0;
    size_t len_size = 0;
    len_size = getline(&input, &len, stdin);
    if (len_size == -1)
        return -1;
    if (len_size < max_size) {
        input[len_size - 1] = '\0';
        strncpy(dst, input, len_size);
    }
    else {
        input[max_size - 1] = '\0';
        strncpy(dst, input, max_size);
        len_size = max_size;
    }
    free(input);
    return len_size;
}


bool isNumber(const char* str, int len) {
    for (int i = 0; i < len-1; i++) {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

int get_int_input() {
    char intStr[10];
    int getInt, len = 0;
    bool isInt = false;
    
    len = get_input(intStr, 10);
    if (intStr == NULL)
        isInt = false;
    else {
        isInt = isNumber(intStr, len);
    }

    while (!isInt) {
        printf("Expecting a number. Try again: ");
        len = get_input(intStr, 10);
        if (intStr == NULL)
            isInt = false;
        else {
            isInt = isNumber(intStr, len);
        }
    }

    getInt = atoi(intStr);
    return getInt;
}

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
void addStudent(Student* newStudent) {
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
Student* findStudent(const char* firstName, const char* lastName) {
    for (int grade = 0; grade < NUM_GRADES; ++grade) {
        for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
            Student* currentStudent = school[grade][classNum];
            while (currentStudent != NULL) {
                if (strcmp(currentStudent->firstName, firstName) == 0 && strcmp(currentStudent->lastName, lastName) == 0) {
                    return currentStudent;
                }
                currentStudent = currentStudent->next;
            }
        }
    }
    return NULL; // Student not found
}


// Function to delete a student from the school system
void deleteStudent() {
    char firstName[50], lastName[50];

    printf("Enter the first name: ");
    get_input(firstName, 50);
    printf("Enter the last name: ");
    get_input(lastName, 50);

    // Find the student in the school system
    Student* targetStudent = findStudent(firstName, lastName);

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
    char confirm, c;
    scanf(" %c", &confirm);
    c = getchar();
    while (c != '\n') c = getchar();


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


bool validMarks(const char* str) {
    int count = 0;
    char* strCopy = strdup(str); 
    char* token = strtok(strCopy, " "); 

    while (token != NULL) {
        int mark = atoi(token);

        // Check if the mark is within the valid range (0 to 100)
        if (mark < 0 || mark > 100 || (strcmp(token, "0") && mark == 0)) {
            free(strCopy); 
            return false;
        }

        count++;
        token = strtok(NULL, " ");
    }

    free(strCopy); 
    return count == 10;
}


// Function to get new student details from the user and perform validation
void addNewStudent() {
    char firstName[50], lastName[50], phoneNumber[15], gradeStr[10], classStr[10], marksStr[50];
    int grade, classNum, marks[10];

    printf("Enter the first name: ");
    get_input(firstName, 50);
    printf("Enter the last name: ");
    get_input(lastName, 50);
    while (findStudent(firstName, lastName) != NULL) {
        printf("Student already exists in school.\n");
        printf("Enter the first name: ");
        get_input(firstName, 50);
        printf("Enter the last name: ");
        get_input(lastName, 50);
    }
    printf("Enter the phone number: ");
    int len = get_input(phoneNumber, 15);
    while (!isNumber(phoneNumber, len)) {
        printf("Error: Expecting a phone number. Try again.\n");
        len = get_input(phoneNumber, 15);
    }
    printf("Enter the grade (1-12): ");
    grade = get_int_input();
    while (grade < 1 || grade > NUM_GRADES) {
        printf("Error: Invalid grade number. Try again.\n");
        grade = get_int_input();
    }
    printf("Enter the class number (1-10): ");
    classNum = get_int_input();
    while (classNum < 1 || classNum > NUM_CLASSES) {
        printf("Error: Invalid class number. Try again.\n");
        classNum = get_int_input();
    }

    printf("Enter the marks for 10 subjects (separated by spaces): ");
    len = get_input(marksStr, 50);
    while (!validMarks(marksStr)) {
        printf("Error: Invalid marks list. Try again.\n");
        len = get_input(marksStr, 50);
    }
    int count = 0;
    char* strCopy = strdup(marksStr); 
    char* token = strtok(strCopy, " "); 
    while (token != NULL && count < 10) {
        marks[count] = atoi(token);
        count++;
        token = strtok(NULL, " ");
    }
    free(strCopy); 

    Student* newStudent = createStudentNode(firstName, lastName, phoneNumber, grade, classNum, marks);
    if (newStudent == NULL) {
        printf("Error: Memory allocation failed.\n");
    } else {
        addStudent(newStudent);
        printf("Student added successfully.\n");
    }
}



// Function to read students from a text file and add them to the school system
void readStudentsFromFile(const char* filename) {
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
                addStudent(newStudent);
            }
        }

    }

    fclose(file);
}


// Function to print all the students in the school
void printAllStudents() {
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
void freeStudents() {
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
void searchStudent() {
    char firstName[50], lastName[50];

    printf("Enter the first name of the student to search: ");
    get_input(firstName, 50);
    printf("Enter the last name of the student to search: ");
    get_input(lastName, 50);

    // Find the student in the school system
    Student* targetStudent = findStudent(firstName, lastName);

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


// Function to edit a student's grade in a certain subject
void editStudentGrade() {
    char firstName[50], lastName[50];

    printf("Enter the first name of the student to edit: ");
    get_input(firstName, 50);
    printf("Enter the last name of the student to edit: ");
    get_input(lastName, 50);

    // Find the student in the school system
    Student* targetStudent = findStudent(firstName, lastName);
    if (targetStudent == NULL) {
        printf("Student not found.\n");
        return;
    }

    int subjectChoice;
    printf("Select the subject to edit the grade:\n");
    printf("[0] Biology\n");
    printf("[1] Algebra\n");
    printf("[2] Chemistry\n");
    printf("[3] Calculus\n");
    printf("[4] English\n");
    printf("[5] History\n");
    printf("[6] Physics\n");
    printf("[7] Grammar\n");
    printf("[8] Geography\n");
    printf("[9] Statistics\n");
    printf("Enter the subject number (0-9): ");
    subjectChoice = get_int_input();

    while (subjectChoice < 0 || subjectChoice > 9) {
        printf("Invalid subject number.\n");
        subjectChoice = get_int_input();
    }

    int newGrade;
    printf("Enter the new grade for the selected subject: ");
    newGrade = get_int_input();

    while (newGrade < 0 || newGrade > 100) {
        printf("Invalid grade. Grade should be between 0 and 100.\n");
        newGrade = get_int_input();
    }

    // Update the grade for the selected subject
    targetStudent->marks[subjectChoice] = newGrade;

    printf("Grade updated successfully.\n");
}

// Function to get the top N students in a specific subject in a specific grade
void printTopNStudentsPerCourse() {
    int grade, subject, topN;

    printf("Enter the grade (1 to 12): ");
    grade = get_int_input();
    while (grade < 1 || grade > NUM_GRADES) {
        printf("Invalid grade.\n");
        grade = get_int_input();
    }

    printf("Select the subject:\n");
    printf("[0] Biology\n");
    printf("[1] Algebra\n");
    printf("[2] Chemistry\n");
    printf("[3] Calculus\n");
    printf("[4] English\n");
    printf("[5] History\n");
    printf("[6] Physics\n");
    printf("[7] Grammar\n");
    printf("[8] Geography\n");
    printf("[9] Statistics\n");
    printf("Enter the subject number (0-9): ");
    subject = get_int_input();
    while (subject < 0 || subject > 9) {
        printf("Invalid subject number.\n");
        subject = get_int_input();
    }

    printf("Enter the number of top students you want to view: ");
    topN = get_int_input();
    while (topN <= 0) {
        printf("Invalid value for topN.\n");
        topN = get_int_input();
    }

    Student* topStudents[topN];
    for (int i = 0; i < topN; ++i) {
        topStudents[i] = NULL;
    }

    for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
        Student* currentStudent = school[grade - 1][classNum];
        while (currentStudent != NULL) {
            if (topStudents[topN - 1] == NULL || currentStudent->marks[subject] > topStudents[topN - 1]->marks[subject]) {
                int j = topN - 2;
                while (j >= 0 && (topStudents[j] == NULL || currentStudent->marks[subject] > topStudents[j]->marks[subject])) {
                    topStudents[j + 1] = topStudents[j];
                    j--;
                }
                topStudents[j + 1] = currentStudent;
            }
            currentStudent = currentStudent->next;
        }
    }

    printf("Top %d students in %s for Grade %d:\n", topN, subjectNames[subject], grade);
    for (int i = 0; i < topN && topStudents[i] != NULL; ++i) {
        printf("%s %s (Mark: %d)\n", topStudents[i]->firstName, topStudents[i]->lastName, topStudents[i]->marks[subject]);
    }
}


// Function to calculate the average of a grade per a certain subject
void printAverage() {
    int grade, subject;

    printf("Enter the grade (1 to 12): ");
    grade = get_int_input();
    while (grade < 1 || grade > NUM_GRADES) {
        printf("Invalid grade.\n");
        grade = get_int_input();
    }

    printf("Select the subject:\n");
    printf("[0] Biology\n");
    printf("[1] Algebra\n");
    printf("[2] Chemistry\n");
    printf("[3] Calculus\n");
    printf("[4] English\n");
    printf("[5] History\n");
    printf("[6] Physics\n");
    printf("[7] Grammar\n");
    printf("[8] Geography\n");
    printf("[9] Statistics\n");
    printf("Enter the subject number (0-9): ");
    subject = get_int_input();
    while (subject < 0 || subject > 9) {
        printf("Invalid subject number.\n");
        subject = get_int_input();
    }

    int totalMarks = 0;
    int totalStudents = 0;

    for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
        Student* currentStudent = school[grade - 1][classNum];
        while (currentStudent != NULL) {
            totalMarks += currentStudent->marks[subject];
            totalStudents++;
            currentStudent = currentStudent->next;
        }
    }

    if (totalStudents == 0) {
        printf("No students found in Grade %d for %s.\n", grade, subjectNames[subject]);
    } else {
        float average = (float)totalMarks / totalStudents;
        printf("Average marks in %s for Grade %d: %.2f\n", subjectNames[subject], grade, average);
    }
}


// Function to print underperformed students (students whose average is below 55)
void printUnderperformedStudents() {
    int underperformedCount = 0;

    for (int grade = 1; grade <= NUM_GRADES; ++grade) {
        for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
            Student* currentStudent = school[grade - 1][classNum];
            while (currentStudent != NULL) {
                float average = 0.0;
                for (int i = 0; i < 10; ++i) {
                    average += currentStudent->marks[i];
                }
                average /= 10;

                if (average < MIN_AVERAGE) {
                    underperformedCount++;
                    printf("Underperformed Student:\n");
                    printf("Name: %s %s\n", currentStudent->firstName, currentStudent->lastName);
                    printf("Grade: %d\n", currentStudent->grade);
                    printf("Class: %d\n", currentStudent->class);
                    printf("Phone Number: %s\n", currentStudent->phoneNumber);
                    printf("Average Marks: %.2f\n\n", average);
                }

                currentStudent = currentStudent->next;
            }
        }
    }

    if (underperformedCount == 0) {
        printf("No underperformed students found.\n");
    }
}

// Function to export the whole school (all students) into a file with a timestamp in the filename
void exportDatabase() {
    time_t now;
    struct tm* timestamp;
    char filename[100];

    time(&now);
    timestamp = localtime(&now);

    strftime(filename, sizeof(filename), "school_data_%Y%m%d_%H%M%S.txt", timestamp);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to create the file '%s'\n", filename);
        return;
    }

    for (int grade = 0; grade < NUM_GRADES; ++grade) {
        for (int classNum = 0; classNum < NUM_CLASSES; ++classNum) {
            Student* currentStudent = school[grade][classNum];
            while (currentStudent != NULL) {
                fprintf(file, "%s %s %s %d %d ", currentStudent->firstName, currentStudent->lastName,
                        currentStudent->phoneNumber, currentStudent->grade, currentStudent->class);

                for (int i = 0; i < 10; ++i) {
                    fprintf(file, "%d ", currentStudent->marks[i]);
                }

                fprintf(file, "\n");
                currentStudent = currentStudent->next;
            }
        }
    }

    fclose(file);
    printf("School data exported to '%s' successfully.\n", filename);
}


void menu() {
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
                addNewStudent();
                break;
            case Delete:
                deleteStudent();
                break;
            case Edit:
                editStudentGrade();
                break;
            case Search:
                searchStudent();
                break;
            case Showall:
                printAllStudents();
                break;
            case Top10:
                printTopNStudentsPerCourse();
                break;
            case UnderperformedStudents:
                printUnderperformedStudents();
                break;
            case Average:
                printAverage();
                break;
            case Export:
                exportDatabase();
                break;
            case Exit:
                freeStudents();
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
    const char* filename = "students_with_class.txt";
    readStudentsFromFile(filename);

    menu();


    return 0;
}