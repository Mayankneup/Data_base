Employee Performance Management System
This is a robust command-line Employee Performance Management System written in C. It offers a comprehensive set of features for managing employee records, including real-time CRUD operations, performance-based sorting, efficient data persistence, and enhanced user interaction.

Features
Modular CRUD Operations: Easily add, update, and remove employee records with a streamlined command-line interface.
Dynamic Data Management: Efficiently handles varying dataset sizes using dynamic memory allocation (malloc, realloc, free), capable of managing over 1,000 employee entries.
Performance Scoring: Automatically calculates and ranks employee performance based on a customizable algorithm leveraging salary and years of experience.
Persistent Data Storage: Ensures data integrity and continuity across sessions by saving and loading employee information to/from a binary file (employees.dat) using fwrite and fread.
Enhanced Search & Display: Quickly search for employees by name (case-insensitive) and display all records sorted by performance score for insightful analysis.
Robust Input Handling: Incorporates input validation and a cleared input buffer for a more reliable and user-friendly experience.
User Confirmation: Includes confirmation prompts for critical operations like employee removal to prevent accidental data loss.
Data Storage
Employee data is persistently stored in a binary file named employees.dat. The system automatically loads this data upon startup and saves any changes when the program exits, ensuring no data is lost between sessions.

Structure
The main() function orchestrates the primary program loop and manages user interactions.
Functions are modularized for clear separation of concerns, covering CRUD operations, file I/O, and data processing.
Leverages dynamic memory allocation to efficiently manage the employee data array in memory, adapting to the number of stored records.
Compile and Run
To compile and run the Employee Performance Management System, follow these steps:

Bash
gcc -o employee_manager main.c
./employee_manager
