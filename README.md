# 📚 Library Management System 

> A feature-rich, console-based Library Management System built in **C++** with colored terminal UI, due-date tracking, fine calculation, and a fully organized menu system.

---

## 📌 Overview

The **Library Management System** is a C++ command-line application that helps librarians efficiently manage books and members. It supports adding, searching, issuing, and returning books — complete with **due date enforcement**, **overdue fine calculation**, and a beautifully formatted **colored console interface**.

---

## ✨ Features

### 📖 Book Management
- Add books with **Title**, **Author**, and **Genre**
- Remove books by **ID** or **Name**
- Search books by **ID** or **Name**
- View **all books**, **available books only**, or **currently issued books**

### 👥 Member Management
- Register members with **Name** and **Email**
- Remove members by **ID** or **Name**
- Search members by **ID** or **Name**
- View all registered members with their issued book list

### 🔄 Lending System
- Issue a book to a member (auto-assigns a **due date**)
- Return a book from a member
- **Maximum 3 books** per member enforced
- **14-day loan period** with **Rs. 5/day overdue fine**

### 📊 Reports & Statistics
- Total books, available books, issued books count
- Total registered members
- Total pending fine amount across all issued books

### 🎨 Enhanced UI
- **Colored terminal output** (ANSI escape codes)
- Color-coded messages: `[OK]` ✅ green, `[ERR]` ❌ red, `[WRN]` ⚠ yellow, `[INF]` ℹ cyan
- **Box-drawn card layout** for books and members
- **Categorized menu** with 4 sections: Book Operations, Member Operations, Lending, Reports
- **Pre-loaded sample data** on startup (6 books + 2 members)

---

## 🖥️ Preview

```
  +==============================================================+
  |                                                              |
  |          LIBRARY  MANAGEMENT  SYSTEM  |
  |          Manage Books & Members Efficiently                  |
  |                                                              |
  +==============================================================+
  Today: 2026-06-18   Loan Period: 14 days   Fine: Rs.5/day overdue

  +-------- BOOK OPERATIONS --------+
  |  [1]  Add Book                    |
  |  [2]  Remove Book by ID            |
  ...
  +------- REPORTS -----------------+
  |  [17] Library Statistics           |
  +---------------------------------+
```

```
  +----------------------------------------------------------+
  |  ID: 1001   Genre: Fiction
  |  Title  : The Great Gatsby
  |  Author : F. Scott Fitzgerald
  |  Status : Available
  +----------------------------------------------------------+
```

---

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| **C++ (C++14)** | Core programming language |
| **STL** (`unordered_map`, `vector`, `shared_ptr`) | Efficient data structures & memory management |
| **OOP** (Classes, Encapsulation, Abstraction) | Modular and maintainable design |
| **ANSI Escape Codes** | Colored terminal output |
| **Windows API** (`windows.h`) | Enable virtual terminal processing for colors |
| **`<chrono>` / `<ctime>`** | Date handling for due dates and fine calculation |

---

## 🚀 Getting Started

### Prerequisites

Ensure you have one of the following installed:

- **MinGW g++** (recommended for Windows) — [Download MinGW](https://www.mingw-w64.org/)
- **MSVC** (Visual Studio C++ compiler)
- **Clang** (with Windows support)

> **Note:** This project uses Windows-specific headers (`windows.h`) for terminal color support. It is designed for **Windows** systems.

---

### Installation & Compilation

**1. Clone the Repository**
```sh
git clone https://github.com/your-username/library-management-system.git
cd library-management-system
```

**2. Compile the Program**
```sh
g++ -std=c++14 -o output/library.exe libraryManagementSystem.cpp
```

**3. Run the Program**
```sh
output\library.exe
```

> On first launch, 6 sample books and 2 members are pre-loaded automatically.

---

## 📖 Menu Options

The program is organized into **4 categories**:

### 📚 Book Operations
| Option | Action |
|--------|--------|
| `[1]`  | Add a new book (Title + Author + Genre) |
| `[2]`  | Remove book by ID |
| `[3]`  | Remove book by Name |
| `[4]`  | Search book by ID |
| `[5]`  | Search book by Name |
| `[6]`  | Show all books |
| `[7]`  | Show only available books |
| `[8]`  | Show only issued books |

### 👥 Member Operations
| Option | Action |
|--------|--------|
| `[9]`  | Register a new member (Name + Email) |
| `[10]` | Remove member by ID |
| `[11]` | Remove member by Name |
| `[12]` | Search member by ID |
| `[13]` | Search member by Name |
| `[14]` | Show all members |

### 🔄 Lending Operations
| Option | Action |
|--------|--------|
| `[15]` | Issue a book to a member |
| `[16]` | Return a book from a member |

### 📊 Reports
| Option | Action |
|--------|--------|
| `[17]` | View library statistics |
| `[0]`  | Exit the program |

---

## 🗂️ Project Structure

```
library-management-system/
│
├── libraryManagementSystem.cpp   # Main source file (all classes & logic)
├── output/
│   └── library.exe               # Compiled executable (after build)
└── README.md                     # Project documentation
```

---

## 🧩 Class Design

```
Book
 ├── bookID, title, author, genre
 ├── isIssued, issueDate, dueDate
 ├── issueBook() / returnBook()
 └── calculateFine()

Member
 ├── memberID, name, email
 ├── vector<shared_ptr<Book>> books
 ├── issueBook() / returnBook()
 └── showDetails()

Library
 ├── unordered_map<int, Book>   books
 ├── unordered_map<int, Member> members
 ├── addBook / removeBook / searchBook / showBooks
 ├── addMember / removeMember / searchMember / showMembers
 ├── issueBook / returnBook
 └── showStats()
```

---

## 📋 Business Rules

- Each member can hold a **maximum of 3 books** at a time
- Loan period is **14 days** from the issue date
- Overdue fine is **Rs. 5 per day** after the due date
- Fine is automatically calculated and displayed when returning an overdue book
- Books cannot be issued to multiple members simultaneously

---

## 🔗 Future Improvements

- 💾 **File/Database Persistence** — Save and load library data across sessions
- 🌐 **Web or GUI Interface** — Browser-based or desktop UI
- 📧 **Email Notifications** — Alert members before due dates
- 📊 **Advanced Reports** — Most borrowed books, active members, monthly summaries
- 🔐 **Admin Authentication** — Password-protected admin access
- 🔍 **Partial Search** — Case-insensitive and fuzzy title/name search

---

## 🤝 Contributing

Contributions are welcome!

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Commit your changes: `git commit -m "Add your feature"`
4. Push to the branch: `git push origin feature/your-feature`
5. Open a Pull Request

