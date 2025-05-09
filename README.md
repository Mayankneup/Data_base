# Employee Management System

A simple command-line Employee Performance Management System written in C. This program allows users to manage employee records with features like adding, updating, removing, searching, sorting, and saving employee data.

##Features

- Add new employees
- Update existing employee details
- Remove employees by ID
- Display all employees sorted by performance score
- Search employees by name
- Automatically calculate a performance score
- Save and load data using a binary file (`employees.dat`) for persistence

## Data Storage

- Employee data is stored in a binary file called `employees.dat`.
- Data is automatically loaded when the program starts and saved when the program exits.

## Structure

- `main()` handles the program loop and user input.
- Functions are modularized for CRUD operations and file I/O.
- Uses dynamic memory allocation (`malloc`, `realloc`, `free`) to manage employee data in memory.

## Compile and Run

```bash
gcc -o employee_manager main.c
./employee_manager
