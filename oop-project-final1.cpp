#include <iostream>
#include <string>
using namespace std;

// ---------- STRUCTURE ----------
// Structure to hold date information
struct Date {
    int day, month, year;
};

// ---------- CATEGORY MANAGER CLASS ----------
// Handles predefined categories and validates them
class CategoryManager {
public:
    static void showCategories() {
        cout << "\nAvailable Categories: Food, Transport, Bills, Shopping, Other\n";
    }

    static bool isValidCategory(const string& cat) {
        return cat == "Food" || cat == "Transport" || cat == "Bills" || cat == "Shopping" || cat == "Other";
    }
};

// ---------- BASE CLASS ----------
// Abstract base class for Income and Expense
class Transaction {
protected:
    double amount;
    string description;
    Date date;
    string category;

public:
    Transaction(double amt, string desc, Date d, string cat)
        : amount(amt), description(desc), date(d), category(cat) {}

    // Display transaction details
    virtual void display() {
        cout << "PKR " << amount << " - " << description << " [" << category << "]"
             << " (" << date.day << "/" << date.month << "/" << date.year << ")\n";
    }

    virtual double getAmount() = 0; // Pure virtual function
    virtual ~Transaction() {}
};

// ---------- INCOME CLASS ----------
// Derived from Transaction, represents income
class Income : public Transaction {
public:
    Income(double amt, string desc, Date d, string cat)
        : Transaction(amt, desc, d, cat) {}

    double getAmount() override {
        return amount;
    }

    void display() override {
        cout << "[INCOME] ";
        Transaction::display();
    }
};

// ---------- EXPENSE CLASS ----------
// Derived from Transaction, represents expense
class Expense : public Transaction {
public:
    Expense(double amt, string desc, Date d, string cat)
        : Transaction(amt, desc, d, cat) {}

    double getAmount() override {
        return amount;
    }

    void display() override {
        cout << "[EXPENSE] ";
        Transaction::display();
    }
};

// ---------- BUDGET CLASS ----------
// Handles budget limit and checks against expenses
class Budget {
private:
    double limit;
public:
    Budget(double l = 0) : limit(l) {}

    void setLimit(double l) {
        limit = l;
    }

    bool isOverBudget(double totalExpenses) const {
        return totalExpenses > limit;
    }

    double getLimit() const {
        return limit;
    }
};

// ---------- REPORT CLASS ----------
// Generates and displays monthly financial summary
class Report {
public:
    static void generate(double income, double expenses, const Budget& b) {
        cout << "\n======= Monthly Financial Report =======\n";
        cout << "Total Income: PKR " << income << endl;
        cout << "Total Expenses: PKR " << expenses << endl;
        cout << "Remaining Balance: PKR " << (income - expenses) << endl;

        if (b.getLimit() > 0) {
            cout << "Budget Limit: PKR " << b.getLimit() << endl;
            cout << (b.isOverBudget(expenses) ? "You exceeded your budget!" : "You are within your budget.") << endl;

          }

        cout << "=======================================\n";
    }
};

// ---------- TRACKER CLASS ----------
// Main class to manage incomes, expenses, and budgeting
class FinanceTracker {
private:
    double incomeAmount = 0;
    double totalExpenses = 0;
    Budget budget;

public:
    void setBudgetLimit(double limit) {
        budget.setLimit(limit);
    }

    void addIncome(double amt, const string& desc, const Date& d, const string& cat) {
        Income income(amt, desc, d, cat);
        income.display();
        incomeAmount = income.getAmount();
    }

    void addExpense(double amt, const string& desc, const Date& d, const string& cat) {
        Expense exp(amt, desc, d, cat);
        exp.display();
        totalExpenses += exp.getAmount();
    }

    void showBalance() const {
        double balance = incomeAmount - totalExpenses;
        cout << "\nTotal Income: PKR " << incomeAmount;
        cout << "\nTotal Expenses: PKR " << totalExpenses;
        cout << "\nRemaining Balance: PKR " << balance << endl;
    }

    void generateReport() const {
        Report::generate(incomeAmount, totalExpenses, budget);
    }

    friend void showFriendView(const FinanceTracker& f);
};

// ---------- FRIEND FUNCTION ----------
// Friend function to access private data of FinanceTracker
void showFriendView(const FinanceTracker& f) {
    cout << "\n[Friend View] Income: PKR " << f.incomeAmount
         << ", Expenses: PKR " << f.totalExpenses << endl;
}

// ---------- MAIN FUNCTION ----------
int main() {
    FinanceTracker tracker;
    double income;
    Date incomeDate;
    string incomeCat;
    double budgetLimit;

    cout << "******* FINANCE TRACKING SYSTEM *******" << endl << endl;

    // Set budget
    cout << "Enter your budget limit for the month: ";
    cin >> budgetLimit;
    tracker.setBudgetLimit(budgetLimit);

    // Input income and date
    cout << "Enter your monthly salary (in PKR): ";
    cin >> income;
    cout << "Enter income date (day month year): ";
    cin >> incomeDate.day >> incomeDate.month >> incomeDate.year;
    cin.ignore();
    incomeCat = "Salary";
    tracker.addIncome(income, "Salary", incomeDate, incomeCat);

    // Input expenses
    int n;
    cout << "\nHow many expenses do you want to enter? ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; ++i) {
        double amt;
        string desc, cat;
        Date expenseDate;

        cout << "\nEnter expense description: ";
        getline(cin, desc);
        cout << "Enter amount for " << desc << " (in PKR): ";
        cin >> amt;
        cout << "Enter date for " << desc << " (day month year): ";
        cin >> expenseDate.day >> expenseDate.month >> expenseDate.year;
        cin.ignore();

        CategoryManager::showCategories();
        cout << "Enter category: ";
        getline(cin, cat);

        if (!CategoryManager::isValidCategory(cat)) {
            cout << "Invalid category! Setting to 'Other'.\n";
            cat = "Other";
        }

        tracker.addExpense(amt, desc, expenseDate, cat);
    }

    // Show summary
    tracker.showBalance();
    showFriendView(tracker);
    tracker.generateReport();

    return 0;
}

