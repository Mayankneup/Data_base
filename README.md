# Employee Performance Management System

A robust command-line Employee Performance Management System written in **C**, designed to manage employee records with features such as real-time CRUD operations, performance-based sorting, dynamic memory management, and persistent binary file storage.

---

## 🚀 Features

- **Modular CRUD Operations**  
  Add, update, and delete employee records seamlessly via an intuitive command-line interface.

- **Dynamic Data Management**  
  Handles growing datasets efficiently using dynamic memory allocation (`malloc`, `realloc`, `free`) — tested with over 1,000 entries.

- **Performance Scoring System**  
  Calculates and ranks employee performance using a customizable algorithm based on salary and years of experience.

- **Persistent Binary File Storage**  
  Saves and loads employee records to/from a binary file (`employees.dat`) using `fwrite()` and `fread()` to preserve data across sessions.

- **Enhanced Search & Display**  
  Quickly search for employees by name (case-insensitive) and list all employees sorted by performance score for performance insights.

- **Robust Input Handling**  
  Implements input validation and clears input buffers to prevent invalid entries and buffer overflows.

- **User Confirmation on Destructive Actions**  
  Adds a safety prompt before critical actions like record deletions to avoid unintended data loss.

---

## 💾 Data Storage

- Employee records are stored in a binary file named `employees.dat`.
- The program automatically loads this file at startup and writes to it upon exiting, ensuring data persistence across sessions.

---

## 🧩 Structure

- `main()` manages the main program loop and user interaction.
- Functional decomposition is used to separate:
  - CRUD operations
  - File I/O logic
  - Sorting and searching
- The system dynamically expands or shrinks the employee array as needed to accommodate the current number of records.

---

## 🛠️ Technologies Used

- **Language:** C  
- **Concepts:** Dynamic Memory Management, File I/O, CLI UI, Algorithm Design  
- **Tools:** GCC Compiler, Linux Terminal (or Windows Subsystem for Linux)

---

## 🧪 Compile and Run

```bash
gcc -o employee_manager main.c
./employee_manager
