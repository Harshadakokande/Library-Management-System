#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <windows.h>

// ─── Enable ANSI colors on Windows ───────────────────────────────────────────
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void enableColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// ─── ANSI Color Palette ───────────────────────────────────────────────────────
namespace Color {
    const std::string RESET          = "\033[0m";
    const std::string BOLD           = "\033[1m";
    const std::string DIM            = "\033[2m";
    const std::string RED            = "\033[31m";
    const std::string GREEN          = "\033[32m";
    const std::string YELLOW         = "\033[33m";
    const std::string BLUE           = "\033[34m";
    const std::string MAGENTA        = "\033[35m";
    const std::string CYAN           = "\033[36m";
    const std::string WHITE          = "\033[37m";
    const std::string BRIGHT_RED     = "\033[91m";
    const std::string BRIGHT_GREEN   = "\033[92m";
    const std::string BRIGHT_YELLOW  = "\033[93m";
    const std::string BRIGHT_BLUE    = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN    = "\033[96m";
}

// ─── Constants ────────────────────────────────────────────────────────────────
static int ID_FOR_BOOK   = 1001;
static int ID_FOR_MEMBER = 2001;
const  int MAX_BOOKS_PER_MEMBER = 3;
const  int LOAN_DAYS            = 14;
const  double FINE_PER_DAY      = 5.0;

static std::string divider(62, '=');
static std::string separator(62, '-');

// ─── Utility: Date helpers ────────────────────────────────────────────────────
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_ptr = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%d");
    return oss.str();
}

std::string getDueDate(int days = LOAN_DAYS) {
    auto now = std::chrono::system_clock::now();
    now += std::chrono::hours(24 * days);
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_ptr = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%d");
    return oss.str();
}

int daysDiff(const std::string& from, const std::string& to) {
    std::tm t1 = {}, t2 = {};
    int y1,m1,d1,y2,m2,d2;
    sscanf(from.c_str(), "%d-%d-%d", &y1, &m1, &d1);
    sscanf(to.c_str(),   "%d-%d-%d", &y2, &m2, &d2);
    t1.tm_year = y1-1900; t1.tm_mon = m1-1; t1.tm_mday = d1;
    t2.tm_year = y2-1900; t2.tm_mon = m2-1; t2.tm_mday = d2;
    std::time_t time1 = std::mktime(&t1);
    std::time_t time2 = std::mktime(&t2);
    return (int)((time2 - time1) / (60 * 60 * 24));
}

// ─── Utility: Styled print helpers ───────────────────────────────────────────
void printSuccess(const std::string& msg) {
    std::cout << Color::BRIGHT_GREEN << "  [OK] " << msg << Color::RESET << "\n";
}
void printError(const std::string& msg) {
    std::cout << Color::BRIGHT_RED << "  [ERR] " << msg << Color::RESET << "\n";
}
void printInfo(const std::string& msg) {
    std::cout << Color::BRIGHT_CYAN << "  [INF] " << msg << Color::RESET << "\n";
}
void printWarning(const std::string& msg) {
    std::cout << Color::BRIGHT_YELLOW << "  [WRN] " << msg << Color::RESET << "\n";
}

// =============================================================================
//  CLASS: Book
// =============================================================================
class Book {
private:
    int         bookID;
    std::string title;
    std::string author;
    std::string genre;
    std::string issueDate;
    std::string dueDate;

public:
    bool isIssued = false;

    Book(std::string title, std::string author = "Unknown", std::string genre = "General")
        : bookID(ID_FOR_BOOK++), title(title), author(author), genre(genre) {}

    int         getBookID()   const { return bookID; }
    std::string getBookName() const { return title;  }
    std::string getAuthor()   const { return author; }
    std::string getGenre()    const { return genre;  }
    std::string getDueDate()  const { return dueDate; }

    void issueBook() {
        isIssued  = true;
        issueDate = getCurrentDate();
        dueDate   = ::getDueDate();
    }

    void returnBook() {
        isIssued  = false;
        issueDate = "";
        dueDate   = "";
    }

    double calculateFine() const {
        if (!isIssued || dueDate.empty()) return 0.0;
        int overdue = daysDiff(dueDate, getCurrentDate());
        return (overdue > 0) ? overdue * FINE_PER_DAY : 0.0;
    }

    void showDetails() const {
        std::string avail = isIssued
            ? Color::BRIGHT_RED + "Issued" + Color::RESET
            : Color::BRIGHT_GREEN + "Available" + Color::RESET;

        std::cout << Color::CYAN << "  +----------------------------------------------------------+\n" << Color::RESET;
        std::cout << Color::CYAN << "  |" << Color::RESET
                  << Color::BOLD << "  ID: " << bookID << Color::RESET
                  << Color::DIM  << "   Genre: " << genre << Color::RESET << "\n";
        std::cout << Color::CYAN << "  |" << Color::RESET
                  << "  Title  : " << Color::BOLD << Color::WHITE << title << Color::RESET << "\n";
        std::cout << Color::CYAN << "  |" << Color::RESET
                  << "  Author : " << author << "\n";
        std::cout << Color::CYAN << "  |" << Color::RESET
                  << "  Status : " << avail;
        if (isIssued) {
            std::cout << Color::RESET << "   Due: " << Color::BRIGHT_YELLOW << dueDate << Color::RESET;
            double fine = calculateFine();
            if (fine > 0)
                std::cout << "   " << Color::BRIGHT_RED << "Fine: Rs." << (int)fine << Color::RESET;
        }
        std::cout << "\n";
        std::cout << Color::CYAN << "  +----------------------------------------------------------+\n" << Color::RESET;
    }
};

// =============================================================================
//  CLASS: Member
// =============================================================================
class Member {
private:
    int         memberID;
    std::string name;
    std::string email;
    std::vector<std::shared_ptr<Book>> books;

public:
    Member(std::string name, std::string email = "N/A")
        : memberID(ID_FOR_MEMBER++), name(name), email(email) {}

    std::string getName()     const { return name;     }
    std::string getEmail()    const { return email;    }
    int         getMemberID() const { return memberID; }
    int         getBookCount()const { return (int)books.size(); }

    void issueBook(std::shared_ptr<Book> book) {
        book->issueBook();
        books.emplace_back(book);
    }

    void returnBook(std::shared_ptr<Book> book) {
        auto it = std::find(books.begin(), books.end(), book);
        if (it == books.end()) {
            printError("This book is not issued to this member!");
            return;
        }
        double fine = book->calculateFine();
        book->returnBook();
        books.erase(it);
        if (fine > 0)
            printWarning("Overdue return! Fine charged: Rs." + std::to_string((int)fine));
        else
            printSuccess("Book returned on time!");
    }

    void showDetails() const {
        std::cout << Color::MAGENTA << "  *============================================================*\n" << Color::RESET;
        std::cout << Color::MAGENTA << "  *" << Color::RESET
                  << Color::BOLD << "  Member ID : " << memberID << Color::RESET << "\n";
        std::cout << Color::MAGENTA << "  *" << Color::RESET
                  << "  Name      : " << Color::BOLD << Color::WHITE << name << Color::RESET << "\n";
        std::cout << Color::MAGENTA << "  *" << Color::RESET
                  << "  Email     : " << email << "\n";
        std::cout << Color::MAGENTA << "  *" << Color::RESET
                  << "  Books Held: " << books.size() << " / " << MAX_BOOKS_PER_MEMBER << "\n";
        std::cout << Color::MAGENTA << "  *============================================================*\n" << Color::RESET;
        if (!books.empty()) {
            std::cout << Color::BOLD << Color::YELLOW << "    >> Issued Books:\n" << Color::RESET;
            for (const auto& b : books) b->showDetails();
        }
        std::cout << "\n";
    }
};

// =============================================================================
//  CLASS: Library
// =============================================================================
class Library {
private:
    std::unordered_map<int, std::shared_ptr<Book>>   books;
    std::unordered_map<int, std::shared_ptr<Member>> members;

public:
    // ── Member Operations ──────────────────────────────────────────────────
    void addMember(std::string name, std::string email = "N/A") {
        if (name.empty()) { printError("Member name cannot be empty!"); return; }
        auto member = std::make_shared<Member>(name, email);
        int id = member->getMemberID();
        members[id] = member;
        printSuccess("Member '" + name + "' registered!  ID: " + std::to_string(id));
    }

    void removeMemberById(int id) {
        auto it = members.find(id);
        if (it != members.end()) { members.erase(it); printSuccess("Member removed!"); return; }
        printError("Member ID " + std::to_string(id) + " not found!");
    }

    void removeMemberByName(std::string memberName) {
        auto it = searchMemberByName(memberName, false);
        if (it != members.end()) { members.erase(it); printSuccess("Member removed!"); return; }
        printError("Member '" + memberName + "' not found!");
    }

    std::unordered_map<int, std::shared_ptr<Member>>::iterator
    searchMemberById(int id, bool isConsole) {
        auto it = members.find(id);
        if (isConsole) {
            if (it != members.end()) it->second->showDetails();
            else printError("Member not found!");
        }
        return it;
    }

    std::unordered_map<int, std::shared_ptr<Member>>::iterator
    searchMemberByName(std::string memberName, bool isConsole) {
        for (auto it = members.begin(); it != members.end(); ++it) {
            if (it->second->getName() == memberName) {
                if (isConsole) it->second->showDetails();
                return it;
            }
        }
        if (isConsole) printError("Member '" + memberName + "' not found!");
        return members.end();
    }

    void showMembers() const {
        if (members.empty()) { printInfo("No members registered yet."); return; }
        std::cout << Color::BOLD << Color::MAGENTA
                  << "\n  ** All Registered Members (" << members.size() << ") **\n"
                  << Color::RESET;
        std::cout << Color::MAGENTA << "  " << divider << "\n" << Color::RESET;
        for (auto& kv : members) kv.second->showDetails();
    }

    // ── Book Operations ────────────────────────────────────────────────────
    void addBook(std::string title, std::string author = "Unknown", std::string genre = "General") {
        if (title.empty()) { printError("Book title cannot be empty!"); return; }
        auto book = std::make_shared<Book>(
            title,
            author.empty() ? "Unknown" : author,
            genre.empty()  ? "General" : genre
        );
        int id = book->getBookID();
        books[id] = book;
        printSuccess("Book '" + title + "' added!  ID: " + std::to_string(id));
    }

    void removeBookById(int id) {
        auto it = books.find(id);
        if (it != books.end()) { books.erase(it); printSuccess("Book removed!"); return; }
        printError("Book ID " + std::to_string(id) + " not found!");
    }

    void removeBookByName(std::string bookName) {
        auto it = searchBookByName(bookName, false);
        if (it != books.end()) { books.erase(it); printSuccess("Book removed!"); return; }
        printError("Book '" + bookName + "' not found!");
    }

    std::unordered_map<int, std::shared_ptr<Book>>::iterator
    searchBookById(int id, bool isConsole) {
        auto it = books.find(id);
        if (isConsole) {
            if (it != books.end()) it->second->showDetails();
            else printError("Book not found!");
        }
        return it;
    }

    std::unordered_map<int, std::shared_ptr<Book>>::iterator
    searchBookByName(std::string bookName, bool isConsole) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->second->getBookName() == bookName) {
                if (isConsole) it->second->showDetails();
                return it;
            }
        }
        if (isConsole) printError("Book '" + bookName + "' not found!");
        return books.end();
    }

    void showBooks() const {
        if (books.empty()) { printInfo("No books in library yet."); return; }
        std::cout << Color::BOLD << Color::CYAN
                  << "\n  ** All Books in Library (" << books.size() << ") **\n"
                  << Color::RESET;
        std::cout << Color::CYAN << "  " << divider << "\n" << Color::RESET;
        for (auto& kv : books) kv.second->showDetails();
    }

    void showAvailableBooks() const {
        std::cout << Color::BOLD << Color::BRIGHT_GREEN
                  << "\n  ** Available Books **\n" << Color::RESET;
        std::cout << Color::BRIGHT_GREEN << "  " << divider << "\n" << Color::RESET;
        int count = 0;
        for (auto& kv : books) {
            if (!kv.second->isIssued) { kv.second->showDetails(); count++; }
        }
        if (count == 0) printInfo("All books are currently issued out!");
        else printInfo(std::to_string(count) + " book(s) available.");
    }

    void showIssuedBooks() const {
        std::cout << Color::BOLD << Color::BRIGHT_YELLOW
                  << "\n  ** Currently Issued Books **\n" << Color::RESET;
        std::cout << Color::BRIGHT_YELLOW << "  " << divider << "\n" << Color::RESET;
        int count = 0;
        for (auto& kv : books) {
            if (kv.second->isIssued) { kv.second->showDetails(); count++; }
        }
        if (count == 0) printInfo("No books are currently issued.");
        else printInfo(std::to_string(count) + " book(s) issued.");
    }

    // ── Lending Operations ─────────────────────────────────────────────────
    void issueBook(int memberId, int bookId) {
        auto mIt = searchMemberById(memberId, false);
        if (mIt == members.end()) { printError("Member ID " + std::to_string(memberId) + " not found!"); return; }
        if (mIt->second->getBookCount() >= MAX_BOOKS_PER_MEMBER) {
            printError("Member has reached max book limit (" + std::to_string(MAX_BOOKS_PER_MEMBER) + " books)!");
            return;
        }
        auto bIt = searchBookById(bookId, false);
        if (bIt == books.end()) { printError("Book ID " + std::to_string(bookId) + " not found!"); return; }
        if (bIt->second->isIssued) { printError("Book is already issued to another member!"); return; }
        mIt->second->issueBook(bIt->second);
        printSuccess("Book '" + bIt->second->getBookName() + "' issued to " + mIt->second->getName() + "!");
        printInfo("Due date: " + bIt->second->getDueDate());
    }

    void returnBook(int memberId, int bookId) {
        auto mIt = searchMemberById(memberId, false);
        if (mIt == members.end()) { printError("Member ID " + std::to_string(memberId) + " not found!"); return; }
        auto bIt = searchBookById(bookId, false);
        if (bIt == books.end()) { printError("Book ID " + std::to_string(bookId) + " not found!"); return; }
        mIt->second->returnBook(bIt->second);
    }

    // ── Reports ────────────────────────────────────────────────────────────
    void showStats() const {
        int total    = (int)books.size();
        int issued   = 0;
        double totalFines = 0.0;
        for (auto& kv : books) {
            if (kv.second->isIssued) { issued++; totalFines += kv.second->calculateFine(); }
        }
        int available = total - issued;

        std::cout << Color::BOLD << Color::BRIGHT_BLUE
                  << "\n  ============  LIBRARY STATISTICS  ============\n"
                  << Color::RESET;
        std::cout << Color::CYAN   << "  Total Books     : " << Color::WHITE << Color::BOLD << total     << Color::RESET << "\n";
        std::cout << Color::BRIGHT_GREEN  << "  Available Books : " << Color::WHITE << Color::BOLD << available << Color::RESET << "\n";
        std::cout << Color::BRIGHT_YELLOW << "  Issued Books    : " << Color::WHITE << Color::BOLD << issued    << Color::RESET << "\n";
        std::cout << Color::MAGENTA << "  Total Members   : " << Color::WHITE << Color::BOLD << members.size() << Color::RESET << "\n";
        if (totalFines > 0)
            std::cout << Color::BRIGHT_RED << "  Pending Fines   : " << Color::WHITE << Color::BOLD << "Rs." << (int)totalFines << Color::RESET << "\n";
        std::cout << Color::BRIGHT_BLUE
                  << "  ===============================================\n"
                  << Color::RESET;
    }
};

// ─── Print splash header ──────────────────────────────────────────────────────
void printHeader() {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::BRIGHT_CYAN;
    std::cout << "  +==============================================================+\n";
    std::cout << "  |                                                              |\n";
    std::cout << "  |          LIBRARY  MANAGEMENT  SYSTEM  v2.0                  |\n";
    std::cout << "  |          Manage Books & Members Efficiently                  |\n";
    std::cout << "  |                                                              |\n";
    std::cout << "  +==============================================================+\n";
    std::cout << Color::RESET;
    std::cout << Color::BRIGHT_YELLOW << "  Today: " << getCurrentDate()
              << "   Loan Period: " << LOAN_DAYS << " days   Fine: Rs."
              << FINE_PER_DAY << "/day overdue\n" << Color::RESET;
}

// ─── Print categorized menu ───────────────────────────────────────────────────
void printMenu() {
    std::cout << "\n";
    std::cout << Color::BOLD << Color::BRIGHT_BLUE  << "  +-------- BOOK OPERATIONS --------+\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [1]  Add Book                    " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [2]  Remove Book by ID            " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [3]  Remove Book by Name          " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [4]  Search Book by ID            " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [5]  Search Book by Name          " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [6]  Show All Books               " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [7]  Show Available Books         " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::CYAN << "  | " << Color::RESET << " [8]  Show Issued Books            " << Color::CYAN << "|\n" << Color::RESET;
    std::cout << Color::BOLD << Color::BRIGHT_MAGENTA << "  +------- MEMBER OPERATIONS -------+\n" << Color::RESET;
    std::cout << Color::MAGENTA << "  | " << Color::RESET << " [9]  Add Member                   " << Color::MAGENTA << "|\n" << Color::RESET;
    std::cout << Color::MAGENTA << "  | " << Color::RESET << " [10] Remove Member by ID          " << Color::MAGENTA << "|\n" << Color::RESET;
    std::cout << Color::MAGENTA << "  | " << Color::RESET << " [11] Remove Member by Name        " << Color::MAGENTA << "|\n" << Color::RESET;
    std::cout << Color::MAGENTA << "  | " << Color::RESET << " [12] Search Member by ID          " << Color::MAGENTA << "|\n" << Color::RESET;
    std::cout << Color::MAGENTA << "  | " << Color::RESET << " [13] Search Member by Name        " << Color::MAGENTA << "|\n" << Color::RESET;
    std::cout << Color::MAGENTA << "  | " << Color::RESET << " [14] Show All Members             " << Color::MAGENTA << "|\n" << Color::RESET;
    std::cout << Color::BOLD << Color::BRIGHT_YELLOW << "  +------- LENDING OPERATIONS ------+\n" << Color::RESET;
    std::cout << Color::YELLOW << "  | " << Color::RESET << " [15] Issue Book to Member         " << Color::YELLOW << "|\n" << Color::RESET;
    std::cout << Color::YELLOW << "  | " << Color::RESET << " [16] Return Book from Member      " << Color::YELLOW << "|\n" << Color::RESET;
    std::cout << Color::BOLD << Color::BRIGHT_GREEN  << "  +------- REPORTS -----------------+\n" << Color::RESET;
    std::cout << Color::GREEN << "  | " << Color::RESET << " [17] Library Statistics           " << Color::GREEN << "|\n" << Color::RESET;
    std::cout << Color::BOLD << Color::BRIGHT_RED    << "  +------- EXIT --------------------+\n" << Color::RESET;
    std::cout << Color::RED  << "  | " << Color::RESET << " [0]  Exit                         " << Color::RED << "|\n" << Color::RESET;
    std::cout << Color::BOLD << Color::BRIGHT_BLUE   << "  +---------------------------------+\n" << Color::RESET;
    std::cout << Color::BOLD << "  Your choice: " << Color::RESET;
}

// =============================================================================
//  MAIN
// =============================================================================
int main() {
    enableColors();

    Library library;

    // ── Seed sample data ─────────────────────────────────────────────────────
    std::cout << Color::DIM << Color::CYAN << "\n  Loading sample data...\n" << Color::RESET;
    library.addBook("The Great Gatsby",          "F. Scott Fitzgerald", "Fiction");
    library.addBook("To Kill a Mockingbird",     "Harper Lee",          "Fiction");
    library.addBook("1984",                      "George Orwell",       "Dystopian");
    library.addBook("Introduction to Algorithms","CLRS",                "Computer Science");
    library.addBook("Clean Code",                "Robert C. Martin",    "Programming");
    library.addBook("The Pragmatic Programmer",  "Hunt & Thomas",       "Programming");
    library.addMember("Alice Johnson", "alice@example.com");
    library.addMember("Bob Smith",     "bob@example.com");

    printHeader();

    int choice = 0;
    do {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printError("Invalid input. Please enter a number.");
            continue;
        }
        std::cin.ignore();

        int         memberId, bookId;
        std::string title, author, genre, name, email;

        std::cout << "\n";
        switch (choice) {
            // ── Book Operations ────────────────────────────────────────────
            case 1:
                std::cout << Color::BOLD << Color::CYAN << "  -- Add New Book --\n" << Color::RESET;
                std::cout << "  Title  : "; std::getline(std::cin, title);
                std::cout << "  Author : "; std::getline(std::cin, author);
                std::cout << "  Genre  : "; std::getline(std::cin, genre);
                library.addBook(title, author, genre);
                break;
            case 2:
                std::cout << "  Book ID to remove: "; std::cin >> bookId; std::cin.ignore();
                library.removeBookById(bookId);
                break;
            case 3:
                std::cout << "  Book name to remove: "; std::getline(std::cin, title);
                library.removeBookByName(title);
                break;
            case 4:
                std::cout << "  Book ID to search: "; std::cin >> bookId; std::cin.ignore();
                library.searchBookById(bookId, true);
                break;
            case 5:
                std::cout << "  Book title to search: "; std::getline(std::cin, title);
                library.searchBookByName(title, true);
                break;
            case 6:
                library.showBooks();
                break;
            case 7:
                library.showAvailableBooks();
                break;
            case 8:
                library.showIssuedBooks();
                break;

            // ── Member Operations ──────────────────────────────────────────
            case 9:
                std::cout << Color::BOLD << Color::MAGENTA << "  -- Add New Member --\n" << Color::RESET;
                std::cout << "  Name  : "; std::getline(std::cin, name);
                std::cout << "  Email : "; std::getline(std::cin, email);
                library.addMember(name, email);
                break;
            case 10:
                std::cout << "  Member ID to remove: "; std::cin >> memberId; std::cin.ignore();
                library.removeMemberById(memberId);
                break;
            case 11:
                std::cout << "  Member name to remove: "; std::getline(std::cin, name);
                library.removeMemberByName(name);
                break;
            case 12:
                std::cout << "  Member ID to search: "; std::cin >> memberId; std::cin.ignore();
                library.searchMemberById(memberId, true);
                break;
            case 13:
                std::cout << "  Member name to search: "; std::getline(std::cin, name);
                library.searchMemberByName(name, true);
                break;
            case 14:
                library.showMembers();
                break;

            // ── Lending Operations ─────────────────────────────────────────
            case 15:
                std::cout << Color::BOLD << Color::YELLOW << "  -- Issue Book --\n" << Color::RESET;
                std::cout << "  Member ID : "; std::cin >> memberId; std::cin.ignore();
                std::cout << "  Book ID   : "; std::cin >> bookId;   std::cin.ignore();
                library.issueBook(memberId, bookId);
                break;
            case 16:
                std::cout << Color::BOLD << Color::YELLOW << "  -- Return Book --\n" << Color::RESET;
                std::cout << "  Member ID : "; std::cin >> memberId; std::cin.ignore();
                std::cout << "  Book ID   : "; std::cin >> bookId;   std::cin.ignore();
                library.returnBook(memberId, bookId);
                break;

            // ── Reports ────────────────────────────────────────────────────
            case 17:
                library.showStats();
                break;

            case 0:
                std::cout << Color::BOLD << Color::BRIGHT_GREEN
                          << "\n  Thank you for using Library Management System!\n"
                          << Color::RESET;
                break;
            default:
                printError("Invalid choice! Enter a number from 0 to 17.");
        }

    } while (choice != 0);

    return 0;
}
