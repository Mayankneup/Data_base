#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define MAX_DEPT_LEN 50

//Employee structure
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

int main() {
    Employee *employees = NULL;
    int count = 0;
    int choice;

    loadFromFile(&employees, &count);

    do {
        printf("\nEmployee Performance Management System\n");
        printf("1. Add Employee\n");
        printf("2. Update Employee\n");
        printf("3. Remove Employee\n");
        printf("4. Display All Employees\n");
        printf("5. Search Employee by Name\n");
        printf("6. Exit and Save\n");        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n'); 

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
                printf("Exiting...\n");
                freeMemory(&employees, &count);
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void addEmployee(Employee **employees, int *count) {
    *employees = realloc(*employees, (*count + 1) * sizeof(Employee));
    if (*employees == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    Employee *newEmployee = &(*employees)[*count];
    printf("Enter name of employee: ");
    fgets(newEmployee->name, MAX_NAME_LEN, stdin);
    newEmployee->name[strcspn(newEmployee->name, "\n")] = '\0'; 

    printf("Enter employee ID: ");
    scanf("%d", &newEmployee->employeeID);
    while (getchar() != '\n');

    printf("Enter salary: ");
    scanf("%f", &newEmployee->salary);
    while (getchar() != '\n');

    printf("Enter department: ");
    fgets(newEmployee->department, MAX_DEPT_LEN, stdin);
    newEmployee->department[strcspn(newEmployee->department, "\n")] = '\0';

    printf("Enter years of experience: ");
    scanf("%d", &newEmployee->yearsOfExperience);
    while (getchar() != '\n');

    newEmployee->performanceScore = (0.3 * newEmployee->yearsOfExperience) + (0.7 * newEmployee->salary / 1000);

    (*count)++;
    printf("\nEmployee added successfully!\n");
}

void removeEmployee(Employee **employees, int *count) {
    if (*count == 0) {
        printf("No employees to remove.\n");
        return;
    }

    int id;
    printf("Enter Employee ID to remove: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    for (int i = 0; i < *count; i++) {
        if ((*employees)[i].employeeID == id) {
            for (int j = i; j < *count - 1; j++) {
                (*employees)[j] = (*employees)[j + 1];
            }
            (*count)--;
            *employees = realloc(*employees, (*count) * sizeof(Employee));
            if (*employees == NULL && *count > 0) {
                printf("Memory reallocation failed!\n");
                exit(1);
            }
            printf("Employee with ID %d removed successfully.\n", id);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", id);
}

void updateEmployee(Employee *employees, int count) {
    if (count == 0) {
        printf("No employees to update.\n");
        return;
    }

    int id;
    printf("Enter Employee ID to update: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    for (int i = 0; i < count; i++) {
        if (employees[i].employeeID == id) {
            printf("Enter new salary: ");
            scanf("%f", &employees[i].salary);
            while (getchar() != '\n');

            printf("Enter new department: ");
            fgets(employees[i].department, MAX_DEPT_LEN, stdin);
            employees[i].department[strcspn(employees[i].department, "\n")] = '\0';

            employees[i].performanceScore = (0.3 * employees[i].yearsOfExperience) + (0.7 * employees[i].salary / 1000);
            printf("Employee information updated successfully!\n");
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
    printf("\nEmployee Records (sorted by Performance Score):\n");
    for (int i = 0; i < count; i++) {
        printf("\nName: %s\n", employees[i].name);
        printf("Employee ID: %d\n", employees[i].employeeID);
        printf("Salary: %.2f\n", employees[i].salary);
        printf("Department: %s\n", employees[i].department);
        printf("Years of Experience: %d\n", employees[i].yearsOfExperience);
        printf("Performance Score: %.2f\n", employees[i].performanceScore);
    }
}

void searchEmployeeByName(Employee *employees, int count) {
    if (count == 0) {
        printf("No employees to search.\n");
        return;
    }

    char namePart[MAX_NAME_LEN];
    printf("Enter name to search: ");
    fgets(namePart, MAX_NAME_LEN, stdin);
    namePart[strcspn(namePart, "\n")] = '\0';

    int found = 0;
    printf("\nSearch Results:\n");

    for (int i = 0; i < count; i++) {
        if (strstr(employees[i].name, namePart)) {
            found = 1; 
            printf("\nName: %s\n", employees[i].name);
            printf("Employee ID: %d\n", employees[i].employeeID);
            printf("Salary: %.2f\n", employees[i].salary);
            printf("Department: %s\n", employees[i].department);
            printf("Years of Experience: %d\n", employees[i].yearsOfExperience);
            printf("Performance Score: %.2f\n", employees[i].performanceScore);
        }
    }

    if (!found) {
        printf("No employees found with name containing '%s'.\n", namePart);
    }
}

void sortEmployeesByPerformance(Employee *employees, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (employees[i].performanceScore < employees[j].performanceScore) {
                Employee temp = employees[i];
                employees[i] = employees[j];
                employees[j] = temp;
            }
        }
    }
}


void saveToFile(Employee *employees, int count) {
    FILE *file = fopen("employees.dat", "wb");
    if (!file) {
        printf("Failed to open employees.dat for writing.\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file); 
    fwrite(employees, sizeof(Employee), count, file); 
    fclose(file);

    printf("Data saved to employees.dat successfully.\n");
}

void loadFromFile(Employee **employees, int *count) {
    FILE *file = fopen("employees.dat", "rb");
    if (!file) {
        printf("No saved data file (employees.dat) found.\n");
        return;
    }

    fread(count, sizeof(int), 1, file);
    *employees = malloc(*count * sizeof(Employee));
    if (!*employees && *count > 0) {
        printf("Memory allocation failed.\n");
        fclose(file);
        exit(1);
    }

    fread(*employees, sizeof(Employee), *count, file);
    fclose(file);

    printf("Loaded %d employee(s) from employees.dat\n", *count);
}

void freeMemory(Employee **employees, int *count) {
    free(*employees);
    *employees = NULL;
    *count = 0;
}