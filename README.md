# 📊 Employee Performance Management System

A robust, high-performance command-line interface (CLI) application written in **C**. This system manages employee records using dynamic memory scaling, automated performance analytics, and dual-mode data persistence.

---

## 🚀 Key Features

* **Modular CRUD Engine:** Seamlessly add, update, search, and remove records.
* **Dynamic Data Management:** Uses `malloc` and `realloc` for efficient memory scaling—tested with 1,000+ entries.
* **Performance Scoring:** Automatically ranks employees using a weighted algorithm:
    * $Score = (0.3 \times \text{Experience}) + (0.7 \times \frac{\text{Salary}}{1000})$
* **Dual Persistence:** * **Binary:** Fast I/O via `employees.dat`.
    * **SQL Export:** Generates an `employees.sql` script for database migration.
* **Search & Analytics:** Case-insensitive search and `qsort` performance ranking.

---

## 🛠 Technical Stack

* **Language:** C (C99/C11)
* **Concepts:** Pointer Manipulation, File I/O, Dynamic Arrays, String Sanitization
* **Tools:** GCC Compiler, Linux Terminal/WSL

---

## 📂 Project Structure

| Component | Responsibility |
| :--- | :--- |
| `main.c` | Core logic and memory management |
| `employees.dat` | Binary storage for session persistence |
| `employees.sql` | SQL export (generated on exit) |

---

## ⚙️ Installation & Usage

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git](https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git)
   cd YOUR_REPO_NAME
