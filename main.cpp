#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class BankAccount {
private:
    double balance;
    std::string txFile;

    // Append a transaction line to the file
    void saveTransaction(const std::string& type,
                         double amount,
                         const std::string& item = "")
    {
        std::ofstream file(txFile, std::ios::app);
        if (!file) {
            std::cerr << "[Error] Could not open " << txFile << " for writing.\n";
            return;
        }
        file << std::fixed << std::setprecision(2);
        file << type << ',' << (item.empty() ? "-" : item) << ','
             << amount << ',' << balance << '\n';
    }

public:
    // Constructor starts with $0.00 and (re)creates the transactions file with a header.
    explicit BankAccount(const std::string& filename = "transactions.txt")
        : balance(0.0), txFile(filename)
    {
        std::ofstream file(txFile, std::ios::trunc);
        if (file) {
            file << "Type,Item,Amount,BalanceAfter\n";
        }
    }

    // Add money to the account
    void deposit(double amount) {
        if (amount <= 0) {
            std::cout << "Deposit must be positive.\n";
            return;
        }
        balance += amount;
        std::cout << "Added starting balance of $" << std::fixed << std::setprecision(2)
                  << amount << '\n';
        saveTransaction("Deposit", amount);
    }

    // Attempt a purchase (deducts from balance if sufficient)
    void makePurchase(const std::string& item, double cost) {
        if (cost <= 0) {
            std::cout << "Purchase amount must be positive.\n";
            return;
        }
        if (cost > balance) {
            std::cout << "Insufficient funds for: " << item << " ($"
                      << std::fixed << std::setprecision(2) << cost << ")\n";
            return;
        }
        balance -= cost;
        std::cout << "Purchased \"" << item << "\" for $"
                  << std::fixed << std::setprecision(2) << cost << '\n';
        saveTransaction("Purchase", cost, item);
    }

    // Print current balance
    void displayBalance() const {
        std::cout << "Current Balance: $" << std::fixed << std::setprecision(2)
                  << balance << '\n';
    }
};

int main() {
    BankAccount myAccount;              // Creates account, clears/headers transactions.txt

    myAccount.deposit(100.00);          // Initial deposit
    myAccount.makePurchase("Coffee", 4.50);
    myAccount.makePurchase("Book", 25.00);

    myAccount.displayBalance();         // Print final balance

    // Add at least one more purchase to satisfy "at least two purchases"
    myAccount.makePurchase("Notebook", 6.25);
    myAccount.displayBalance();

    return 0;
}
