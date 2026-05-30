#include "account/Account.h"
#include <iostream>
#include <iomanip>

bool Account::isValidIban(const std::string& iban) {
    return iban.size() >= 15 && iban.size() <= 34
        && std::isalpha(iban[0]) && std::isalpha(iban[1])
        && std::isdigit(iban[2]) && std::isdigit(iban[3]);
}

bool Account::isValidCurrency(const std::string& currency) {
    return currency.size() == 3
        && std::isupper(currency[0])
        && std::isupper(currency[1])
        && std::isupper(currency[2]);
}

Account::Account(const std::string& iban, const std::string& currency, double balance)
    : iban(iban), currency(currency), balance(balance), status(AccountStatus::ACTIVE), transactions() {
    if (!isValidIban(iban))
        throw std::invalid_argument("Invalid IBAN");
    if (!isValidCurrency(currency))
        throw std::invalid_argument("Invalid currency code");
    if (balance < 0)
        throw std::invalid_argument("Balance cannot be negative");
}

Account::Account(const Account& other)
    : iban(other.iban), currency(other.currency),
      balance(other.balance), status(other.status),
      transactions(other.transactions) {
}

std::string Account::getIban() const noexcept { return iban; }
std::string Account::getCurrency() const noexcept { return currency; }
double Account::getBalance() const noexcept { return balance; }
AccountStatus Account::getStatus() const noexcept { return status; }
const std::vector<Transaction>& Account::getTransactions() const noexcept{return transactions;}

void Account::deposit(double amount, const std::string& description) {
    if (status != AccountStatus::ACTIVE)
        throw std::runtime_error("Account is not active");
    if (amount <= 0)
        throw std::invalid_argument("Deposit amount must be positive");
    balance += amount;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::DEPOSIT,
        amount, currency, iban, "", TransactionStatus::SUCCESSFUL, description
    ));
}

void Account::withdraw(double amount, const std::string& description) {
    if (status != AccountStatus::ACTIVE)
        throw std::runtime_error("Account is not active");
    if (amount <= 0)
        throw std::invalid_argument("Withdraw amount must be positive");
    if (amount > balance)
        throw std::runtime_error("Insufficient funds");
    balance -= amount;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::WITHDRAW,
        amount, currency, iban, "", TransactionStatus::SUCCESSFUL, description
    ));
}

void Account::transfer(Account& other, double amount, const std::string& description) {
    if (status != AccountStatus::ACTIVE)
        throw std::runtime_error("Account is not active");
    if (amount <= 0)
        throw std::invalid_argument("Transfer amount must be positive");
    if (amount > balance)
        throw std::runtime_error("Insufficient funds");
    balance -= amount;
    other.balance += amount;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::TRANSFER,
        amount, currency, iban, other.iban, TransactionStatus::SUCCESSFUL, description
    ));
    other.transactions.push_back(Transaction(
        std::to_string(other.transactions.size() + 1),
        TransactionType::TRANSFER,
        amount, currency, iban, other.iban, TransactionStatus::SUCCESSFUL, description
    ));
}

void Account::lock() {
    if (status == AccountStatus::CLOSED)
        throw std::runtime_error("Cannot lock a closed account");
    status = AccountStatus::LOCKED;
}

void Account::close() {
    if (status == AccountStatus::CLOSED)
        throw std::runtime_error("Account is already closed");
    status = AccountStatus::CLOSED;
}

std::vector<Transaction> Account::getTransactionsByType(TransactionType type) const {
    std::vector<Transaction> result;
    for (const Transaction& t : transactions)
        if (t.getType() == type)
            result.push_back(t);
    return result;
}

std::vector<Transaction> Account::getTransactionsByDateRange(std::time_t from, std::time_t to) const {
    std::vector<Transaction> result;
    for (const Transaction& t : transactions)
        if (t.getTimestamp() >= from && t.getTimestamp() <= to)
            result.push_back(t);
    return result;
}

std::vector<Transaction> Account::getTransactionsByMinAmount(double min) const {
    std::vector<Transaction> result;
    for (const Transaction& t : transactions)
        if (t.getAmount() >= min)
            result.push_back(t);
    return result;
}

std::map<Category, double> Account::getCategoryReport(std::time_t from, std::time_t to) const {
    std::map<Category, double> report;
    for (const Transaction& t : transactions) {
        if (t.getTimestamp() < from || t.getTimestamp() > to) continue;
        if (t.getType() == TransactionType::DEPOSIT) continue;
        if (t.getType() == TransactionType::TRANSFER && t.getSourceIban() != getIban()) continue;
        report[t.getCategory()] += t.getAmount();
    }
    return report;
}

static std::string categoryLabel(Category c) {
    switch (c) {
        case Category::FOOD:          return "Food";
        case Category::TRANSPORT:     return "Transport";
        case Category::UTILITIES:     return "Utilities";
        case Category::ENTERTAINMENT: return "Entertainment";
        case Category::HEALTH:        return "Health";
        case Category::SHOPPING:      return "Shopping";
        default:                      return "Other";
    }
}

void Account::printCategoryReport(std::time_t from, std::time_t to) const {
    auto report = getCategoryReport(from, to);
    double total = 0;
    for (auto& [cat, amount] : report) total += amount;

    std::cout << "=== Category Report ===\n"
              << "IBAN: " << getIban() << "\n";
    if (report.empty()) {
        std::cout << "No expenses in this period.\n";
        return;
    }
    for (auto& [cat, amount] : report) {
        int pct = (total > 0) ? static_cast<int>(amount / total * 100) : 0;
        std::string bar(pct / 5, '#');
        std::cout << std::left << std::setw(14) << categoryLabel(cat)
                  << std::right << std::setw(8) << std::fixed << std::setprecision(2) << amount
                  << " " << getCurrency()
                  << "  [" << bar << std::string(20 - bar.size(), ' ') << "] "
                  << pct << "%\n";
    }
    std::cout << std::string(46, '-') << "\n"
              << std::left << std::setw(14) << "TOTAL"
              << std::right << std::setw(8) << std::fixed << std::setprecision(2) << total
              << " " << getCurrency() << "\n";
}

void Account::printMonthlyStatement(int month, int year) const {
    double totalIncome = 0;
    double totalExpenses = 0;
    int count = 0;

    for (const Transaction& t : transactions) {
        std::time_t ts = t.getTimestamp();
        std::tm* tm = std::localtime(&ts);
        if (tm->tm_mon + 1 != month || tm->tm_year + 1900 != year)
            continue;
        count++;
        if (t.getType() == TransactionType::DEPOSIT)
            totalIncome += t.getAmount();
        else if (t.getType() == TransactionType::WITHDRAW)
            totalExpenses += t.getAmount();
        else if (t.getType() == TransactionType::TRANSFER) {
            if (t.getSourceIban() == getIban())
                totalExpenses += t.getAmount();
            else
                totalIncome += t.getAmount();
        }
    }

    std::cout << "=== Monthly Statement " << month << "/" << year << " ===\n"
              << "IBAN: " << getIban() << "\n"
              << "Transactions: " << count << "\n"
              << "Total income: " << totalIncome << " " << getCurrency() << "\n"
              << "Total expenses: " << totalExpenses << " " << getCurrency() << "\n"
              << "Net: " << (totalIncome - totalExpenses) << " " << getCurrency() << "\n"
              << "Current balance: " << balance << " " << getCurrency() << "\n";
}