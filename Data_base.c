#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#define MAX_NAME_LEN 100
#define MAX_DEPT_LEN 50
#define FILENAME "employees.dat" 

// Employee structure
typedef struct {
    char name[MAX_NAME_LEN];
    int employeeID;
    float salary;
    char department[MAX_DEPT_LEN];
    int yearsOfExperience;
    float performanceScore;
} Employee;

// Function prototypes
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
void clearInputBuffer(); 

int main() {
    Employee *employees = NULL;
    int count = 0;
    int choice;

    loadFromFile(&employees, &count);

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
            case 1:
                addEmployee(&employees, &count);
                break;
            case 2:
                updateEmployee(employees, count);
                break;
            case 3:
                removeEmployee(&employees, &count);
                break;
            case 4:
                displayEmployees(employees, count);
                break;
            case 5:
                searchEmployeeByName(employees, count);
                break;
            case 6:
                saveToFile(employees, count);
                printf("Exiting Employee Performance Management System. Goodbye!\n");
                freeMemory(&employees, &count);
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);

    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

float calculatePerformanceScore(int yearsOfExperience, float salary) {
    return (0.3 * yearsOfExperience) + (0.7 * salary / 1000);
}

void addEmployee(Employee **employees, int *count) {
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
            printf("An employee with ID %d already exists. Please use a unique ID.\n", newID);
            return;
        }
    }

    *employees = realloc(*employees, (*count + 1) * sizeof(Employee));
    if (*employees == NULL) {
        printf("Memory allocation failed during addEmployee!\n");
        exit(1);
    }

    Employee *newEmployee = &(*employees)[*count];
    newEmployee->employeeID = newID; // Assign the validated ID

    printf("Enter name of employee: ");
    fgets(newEmployee->name, MAX_NAME_LEN, stdin);
    newEmployee->name[strcspn(newEmployee->name, "\n")] = '\0';

    printf("Enter salary: ");
    if (scanf("%f", &newEmployee->salary) != 1 || newEmployee->salary < 0) {
        printf("Invalid salary. Please enter a non-negative number.\n");
        clearInputBuffer();
        *employees = realloc(*employees, (*count) * sizeof(Employee));
        return;
    }
    clearInputBuffer();

    printf("Enter department: ");
    fgets(newEmployee->department, MAX_DEPT_LEN, stdin);
    newEmployee->department[strcspn(newEmployee->department, "\n")] = '\0';

    printf("Enter years of experience: ");
    if (scanf("%d", &newEmployee->yearsOfExperience) != 1 || newEmployee->yearsOfExperience < 0) {
        printf("Invalid years of experience. Please enter a non-negative integer.\n");
        clearInputBuffer();
        *employees = realloc(*employees, (*count) * sizeof(Employee));
        return;
    }
    clearInputBuffer();

    newEmployee->performanceScore = calculatePerformanceScore(newEmployee->yearsOfExperience, newEmployee->salary);

    (*count)++;
    printf("\nEmployee '%s' added successfully!\n", newEmployee->name);
}

void removeEmployee(Employee **employees, int *count) {
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

    int found_index = -1;
    for (int i = 0; i < *count; i++) {
        if ((*employees)[i].employeeID == id) {
            found_index = i;
            break;
        }
    }

    if (found_index != -1) {
        char confirm;
        printf("Are you sure you want to remove employee with ID %d? (y/n): ", id);
        if (scanf(" %c", &confirm) != 1 || (tolower(confirm) != 'y' && tolower(confirm) != 'n')) {
            printf("Invalid input. Aborting removal.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        if (tolower(confirm) == 'n') {
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
            Employee *temp = realloc(*employees, (*count) * sizeof(Employee));
            if (temp == NULL) {
                printf("Memory reallocation failed after removal! Data might be inconsistent.\n");
            } else {
                *employees = temp;
            }
        }
        printf("Employee with ID %d removed successfully.\n", id);
    } else {
        printf("Employee with ID %d not found.\n", id);
    }
}

void updateEmployee(Employee *employees, int count) {
    if (count == 0) {
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
        if (employees[i].employeeID == id) {
            int update_choice;
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
                    case 1:
                        printf("Enter new salary: ");
                        if (scanf("%f", &employees[i].salary) != 1 || employees[i].salary < 0) {
                            printf("Invalid salary. Please enter a non-negative number.\n");
                        }
                        clearInputBuffer();
                        break;
                    case 2:
                        printf("Enter new department: ");
                        fgets(employees[i].department, MAX_DEPT_LEN, stdin);
                        employees[i].department[strcspn(employees[i].department, "\n")] = '\0';
                        break;
                    case 3:
                        printf("Enter new years of experience: ");
                        if (scanf("%d", &employees[i].yearsOfExperience) != 1 || employees[i].yearsOfExperience < 0) {
                            printf("Invalid years of experience. Please enter a non-negative integer.\n");
                        }
                        clearInputBuffer();
                        break;
                    case 4:
                        printf("Returning to main menu.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
                employees[i].performanceScore = calculatePerformanceScore(employees[i].yearsOfExperience, employees[i].salary);
                if (update_choice >=1 && update_choice <= 3) {
                     printf("Employee information updated successfully!\n");
                }
            } while (update_choice != 4);
            return; 
        }
    }
    printf("Employee with ID %d not found.\n", id);
}

void displayEmployees(Employee *employees, int count) {
    if (count == 0) {
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
    if (count == 0) {
        printf("No employees to search.\n");
        return;
    }

    char namePart[MAX_NAME_LEN];
    printf("Enter name (or part of name) to search: ");
    fgets(namePart, MAX_NAME_LEN, stdin);
    namePart[strcspn(namePart, "\n")] = '\0';

    for (int i = 0; namePart[i]; i++) {
        namePart[i] = tolower(namePart[i]);
    }

    int found = 0;
    printf("\n--- Search Results ---\n");

    for (int i = 0; i < count; i++) {
        char employeeNameLower[MAX_NAME_LEN];
        strcpy(employeeNameLower, employees[i].name);
        for (int j = 0; employeeNameLower[j]; j++) {
            employeeNameLower[j] = tolower(employeeNameLower[j]);
        }

        if (strstr(employeeNameLower, namePart)) {
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
        printf("No employees found with name containing '%s'.\n", namePart);
    }
}

// Comparison function for qsort (if you decide to use it)
int compareEmployees(const void *a, const void *b) {
    Employee *empA = (Employee *)a;
    Employee *empB = (Employee *)b;
    // Sort in descending order of performance score
    if (empA->performanceScore < empB->performanceScore) return 1;
    if (empA->performanceScore > empB->performanceScore) return -1;
    return 0;
}

void sortEmployeesByPerformance(Employee *employees, int count) {
    // Using bubble sort for simplicity as in original code
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (employees[i].performanceScore < employees[j].performanceScore) {
                Employee temp = employees[i];
                employees[i] = employees[j];
                employees[j] = temp;
            }
        }
    }
    // Alternative using qsort:
    // qsort(employees, count, sizeof(Employee), compareEmployees);
}

void saveToFile(Employee *employees, int count) {
    FILE *file = fopen(FILENAME, "wb"); // Use the constant
    if (!file) {
        perror("Failed to open employees.dat for writing"); // Use perror for better error message
        return;
    }

    fwrite(&count, sizeof(int), 1, file);
    if (count > 0) { // Only write employees if there are any
        fwrite(employees, sizeof(Employee), count, file);
    }
    fclose(file);

    printf("Data saved to %s successfully.\n", FILENAME);
}

void loadFromFile(Employee **employees, int *count) {
    FILE *file = fopen(FILENAME, "rb"); // Use the constant
    if (!file) {
        printf("No saved data file (%s) found. Starting with an empty list.\n", FILENAME);
        return;
    }

    fread(count, sizeof(int), 1, file);

    if (*count > 0) {
        *employees = malloc(*count * sizeof(Employee));
        if (*employees == NULL) {
            printf("Memory allocation failed during loadFromFile!\n");
            fclose(file);
            exit(1);
        }
        fread(*employees, sizeof(Employee), *count, file);
    } else {
        *employees = NULL; // No employees to load, so free any potential garbage pointer
    }
    fclose(file);

    printf("Loaded %d employee(s) from %s\n", *count, FILENAME);
}

void freeMemory(Employee **employees, int *count) {
    free(*employees);
    *employees = NULL;
    *count = 0;
    printf("Memory freed.\n");
}
