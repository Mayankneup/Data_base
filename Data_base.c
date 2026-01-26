#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 100
#define MAX_DEPT_LEN 50
#define FILENAME "employees.dat"

/* Structure of the modle */
typedef struct {
    char name[MAX_NAME_LEN];
    int employeeID;
    float salary;
    char department[MAX_DEPT_LEN];
    int yearsOfExperience;
    float performanceScore;
} Employee;

/* All the functions */
void addEmployee(Employee **employees, int *count);
void updateEmployee(Employee *employees, int count);
void displayEmployees(Employee *employees, int count);
void removeEmployee(Employee **employees, int *count);
void searchEmployeeByName(Employee *employees, int count);
void sortEmployeesByPerformance(Employee *employees, int count);
void freeMemory(Employee **employees, int *count);
void saveToFile(Employee *employees, int count);
void loadFromFile(Employee **employees, int *count);

float calculatePerformanceScore(int yearsOfExperience, float salary);
void clearInputBuffer(void);

/* Helpers */
static void trim_newline(char *s);
static void trim_whitespace(char *s);
static int read_line(char *buf, size_t n);
static void *xrealloc(void *ptr, size_t size);
static int compareEmployees(const void *a, const void *b);

/* main */
int main(void) {
    Employee *employees = NULL;
    int count = 0;
    int choice = 0;
    
    //loads previously saved employee data from disk into memory for reuse of the data
    loadFromFile(&employees, &count);

    // All the possible choices for the model
    do {
        printf("\n--- Employee Performance Management System ---\n");
        printf("1. Add New Employee\n");
        printf("2. Update Employee Details\n");
        printf("3. Remove Employee\n");
        printf("4. Display All Employees (Sorted by Performance)\n");
        printf("5. Search Employee by Name\n");
        printf("6. Exit and Save\n");
        printf("----------------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:        //Add employee
                addEmployee(&employees, &count);    
                break;    
            case 2:        //Update any of the attributes to the employee
                updateEmployee(employees, count);
                break;
            case 3:        //Remove the employee from the memory
                removeEmployee(&employees, &count);
                break;
            case 4:        //Display all the information related to the employee
                displayEmployees(employees, count);
                break;
            case 5:        // Search employee; Based on part of the name not specific
                searchEmployeeByName(employees, count);
                break;
            case 6:
                saveToFile(employees, count);       //Exits the system, But saves all the relevent information to the file
                printf("Exiting Employee Performance Management System. Goodbye!\n");
                freeMemory(&employees, &count);
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);

    return 0;
}

/* Input optimization */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard remaining characters in the input buffer
    }
}

//Remove all the new line character
static void trim_newline(char *s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

//Remove all the white line character
static void trim_whitespace(char *s) {
    if (!s) return;        // do nothing is its already at null

    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;        //Finds the whitespace in the input

    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) end--;    //Removes it by doing backspace until it reaches the previous string
    *end = '\0';                                                     //Assign null terminator at the end to define the end of string

    // If leading whitespace was removed, shift the trimmed string to the front
    // memmove is used because source and destination potentally overlap
    if (start != s) {
        memmove(s, start, (size_t)(end - start + 1));    
    }
}

// Read all the line Returns 1 on success, 0 on failure
static int read_line(char *buf, size_t n) {
    if (!buf || n == 0) return 0;
    if (!fgets(buf, (int)n, stdin)) return 0;
    trim_newline(buf);
    trim_whitespace(buf);
    return 1;
}

// realloc wrapper that guarantees safe memory reallocation
// Terminates program on allocation failure to prevent undefined behavior
static void *xrealloc(void *ptr, size_t size) {
    void *tmp = realloc(ptr, size);
    if (!tmp && size != 0) {
        fprintf(stderr, "Fatal: realloc failed\n");
        exit(1);
    }
    return tmp;
}

/* Buiseness Logics */
float calculatePerformanceScore(int yearsOfExperience, float salary) {
    return (0.3f * (float)yearsOfExperience) + (0.7f * salary / 1000.0f);        // Made of performance formula
}


void addEmployee(Employee **employees, int *count) {
    if (!employees || !count) return;        //Defensive check to prevent null pointer dereference
    int newID;
    printf("Enter employee ID: ");
    if (scanf("%d", &newID) != 1) {
        printf("Invalid ID. Please enter a number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < *count; i++) {
        if ((*employees)[i].employeeID == newID) {
            printf("An employee with ID %d already exists. Use a unique ID.\n", newID);
            return;
        }
    }
    
    // Expand dynamic employee array by one and point to that entry
    *employees = xrealloc(*employees, (size_t)(*count + 1) * sizeof(Employee));
    Employee *e = &(*employees)[*count];

    // Store employee ID in newly allocated record
    e->employeeID = newID;

    printf("Enter name of employee: ");
    if (!read_line(e->name, MAX_NAME_LEN)) return;

    printf("Enter salary: ");
    if (scanf("%f", &e->salary) != 1 || e->salary < 0) {
        printf("Invalid salary. Please enter a non-negative number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    printf("Enter department: ");
    if (!read_line(e->department, MAX_DEPT_LEN)) return;

    printf("Enter years of experience: ");
    if (scanf("%d", &e->yearsOfExperience) != 1 || e->yearsOfExperience < 0) {
        printf("Invalid years of experience. Please enter a non-negative integer.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    e->performanceScore = calculatePerformanceScore(e->yearsOfExperience, e->salary);

    (*count)++;
    printf("Employee '%s' added successfully!\n", e->name);        // Increase employee count
}

void removeEmployee(Employee **employees, int *count) {
    if (!employees || !count) return;             //Defensive check to prevent null pointer dereference

    if (*count == 0) {
        printf("No employees to remove.\n");
        return;
    }

    int id;
    printf("Enter Employee ID to remove: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    // Find out where in the memory is the employee located
    int found_index = -1;        // fail search,
    for (int i = 0; i < *count; i++) {
        if ((*employees)[i].employeeID == id) {
            found_index = i;     // If the person is found it stores the index into found index
            break;
        }
    }

    if (found_index == -1) {        // Output for the fail search; If the employee dont exist this gets exicuted
        printf("Employee with ID %d not found.\n", id);
        return;
    }

    // This whole block is double checking if employee is actually being removec
    char confirm;        
    printf("Are you sure you want to remove employee with ID %d? (y/n): ", id);
    if (scanf(" %c", &confirm) != 1) {
        printf("Invalid input. Aborting removal.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    confirm = (char)tolower((unsigned char)confirm);
    if (confirm != 'y') {
        printf("Employee removal cancelled.\n");
        return;
    }

    for (int j = found_index; j < *count - 1; j++) {
        (*employees)[j] = (*employees)[j + 1];
    }
    (*count)--;

    if (*count == 0) {
        free(*employees);
        *employees = NULL;
    } else {
        *employees = xrealloc(*employees, (size_t)(*count) * sizeof(Employee));
    }

    printf("Employee with ID %d removed successfully.\n", id);
}


void updateEmployee(Employee *employees, int count) {
    if (!employees || count == 0) {
        printf("No employees to update.\n");
        return;
    }

    int id;
    printf("Enter Employee ID to update: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input. Please enter a number.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    for (int i = 0; i < count; i++) {
        if (employees[i].employeeID != id) continue;

        int update_choice = 0;
        do {
            printf("\n--- Update Employee %s (ID: %d) ---\n", employees[i].name, employees[i].employeeID);
            printf("1. Update Salary\n");
            printf("2. Update Department\n");
            printf("3. Update Years of Experience\n");
            printf("4. Go Back to Main Menu\n");
            printf("----------------------------------\n");
            printf("Enter your choice: ");

            if (scanf("%d", &update_choice) != 1) {
                printf("Invalid input. Please enter a number.\n");
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            switch (update_choice) {
                case 1: {
                    float newSalary;
                    printf("Enter new salary: ");
                    if (scanf("%f", &newSalary) != 1 || newSalary < 0) {
                        printf("Invalid salary. Please enter a non-negative number.\n");
                        clearInputBuffer();
                        break;
                    }
                    clearInputBuffer();
                    employees[i].salary = newSalary;
                    employees[i].performanceScore = calculatePerformanceScore(employees[i].yearsOfExperience, employees[i].salary);
                    printf("Salary updated.\n");
                    break;
                }
                case 2:
                    printf("Enter new department: ");
                    if (read_line(employees[i].department, MAX_DEPT_LEN)) {
                        employees[i].performanceScore = calculatePerformanceScore(employees[i].yearsOfExperience, employees[i].salary);
                        printf("Department updated.\n");
                    }
                    break;
                case 3: {
                    int newYears;
                    printf("Enter new years of experience: ");
                    if (scanf("%d", &newYears) != 1 || newYears < 0) {
                        printf("Invalid years of experience. Please enter a non-negative integer.\n");
                        clearInputBuffer();
                        break;
                    }
                    clearInputBuffer();
                    employees[i].yearsOfExperience = newYears;
                    employees[i].performanceScore = calculatePerformanceScore(employees[i].yearsOfExperience, employees[i].salary);
                    printf("Years of experience updated.\n");
                    break;
                }
                case 4:
                    printf("Returning to main menu.\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (update_choice != 4);

        return;
    }

    printf("Employee with ID %d not found.\n", id);
}

void displayEmployees(Employee *employees, int count) {
    if (!employees || count == 0) {
        printf("No employees to display.\n");
        return;
    }

    sortEmployeesByPerformance(employees, count);

    printf("\n--- Employee Records (Sorted by Performance Score) ---\n");
    for (int i = 0; i < count; i++) {
        printf("\nName: %s\n", employees[i].name);
        printf("Employee ID: %d\n", employees[i].employeeID);
        printf("Salary: %.2f\n", employees[i].salary);
        printf("Department: %s\n", employees[i].department);
        printf("Years of Experience: %d\n", employees[i].yearsOfExperience);
        printf("Performance Score: %.2f\n", employees[i].performanceScore);
        printf("----------------------------------------------------\n");
    }
}

void searchEmployeeByName(Employee *employees, int count) {
    if (!employees || count == 0) {
        printf("No employees to search.\n");
        return;
    }

    
    char needle[MAX_NAME_LEN];     // Buffer for name search 
    printf("Enter name (or part of name) to search: ");
    if (!read_line(needle, MAX_NAME_LEN)) return;

    // Convert search to lowercase for case matching
    for (int i = 0; needle[i]; i++) {
        needle[i] = (char)tolower((unsigned char)needle[i]);
    }

    int found = 0;
    printf("\n--- Search Results ---\n");

    for (int i = 0; i < count; i++) {
        char hay[MAX_NAME_LEN];
        strncpy(hay, employees[i].name, MAX_NAME_LEN - 1);
        hay[MAX_NAME_LEN - 1] = '\0';

        for (int j = 0; hay[j]; j++) {
            hay[j] = (char)tolower((unsigned char)hay[j]);
        }

        if (strstr(hay, needle)) {
            found = 1;
            printf("\nName: %s\n", employees[i].name);
            printf("Employee ID: %d\n", employees[i].employeeID);
            printf("Salary: %.2f\n", employees[i].salary);
            printf("Department: %s\n", employees[i].department);
            printf("Years of Experience: %d\n", employees[i].yearsOfExperience);
            printf("Performance Score: %.2f\n", employees[i].performanceScore);
            printf("----------------------------------------------------\n");
        }
    }

    if (!found) {
        printf("No employees found with name containing '%s'.\n", needle);
    }
}

/* Sorting  */
static int compareEmployees(const void *a, const void *b) {
    const Employee *empA = (const Employee *)a;
    const Employee *empB = (const Employee *)b;

    /* descending by performanceScore */
    if (empA->performanceScore < empB->performanceScore) return 1;
    if (empA->performanceScore > empB->performanceScore) return -1;
    return 0;
}

void sortEmployeesByPerformance(Employee *employees, int count) {
    qsort(employees, (size_t)count, sizeof(Employee), compareEmployees);
}

// Saves all the information into binary file
void saveToFile(Employee *employees, int count) {
    FILE *file = fopen(FILENAME, "wb");
    if (!file) {
        perror("Failed to open employees.dat for writing");
        return;
    }

    if (fwrite(&count, sizeof(int), 1, file) != 1) {
        perror("Failed to write count");
        fclose(file);
        return;
    }

    if (count > 0) {
        if (fwrite(employees, sizeof(Employee), (size_t)count, file) != (size_t)count) {
            perror("Failed to write employee records");
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Data saved to %s successfully.\n", FILENAME);
}

//This load all the previous employee inforamtion into the current data base
void loadFromFile(Employee **employees, int *count) {
    if (!employees || !count) return;

    FILE *file = fopen(FILENAME, "rb");
    if (!file) {
        printf("No saved data file (%s) found. Starting with an empty list.\n", FILENAME);
        return;
    }

    int n = 0;
    if (fread(&n, sizeof(int), 1, file) != 1 || n < 0) {
        printf("Data file is corrupted (bad count). Starting empty.\n");
        fclose(file);
        return;
    }

    Employee *tmp = NULL;
    if (n > 0) {
        tmp = (Employee *)malloc((size_t)n * sizeof(Employee));
        if (!tmp) {
            printf("Memory allocation failed during loadFromFile!\n");
            fclose(file);
            exit(1);
        }

        if (fread(tmp, sizeof(Employee), (size_t)n, file) != (size_t)n) {
            printf("Data file is corrupted (bad records). Starting empty.\n");
            free(tmp);
            fclose(file);
            return;
        }
    }

    fclose(file);
    *employees = tmp;
    *count = n;

    printf("Loaded %d employee(s) from %s\n", *count, FILENAME);
}

/* Clean up of memory */
void freeMemory(Employee **employees, int *count) {
    if (!employees || !count) return;
    free(*employees);
    *employees = NULL;
    *count = 0;
    printf("Memory freed.\n");
}
