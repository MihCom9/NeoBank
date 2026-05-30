#pragma once
#include <string>
#include <stdexcept>
#include <map>
#include "enums/AccountStatus.h"
#include "enums/Category.h"
#include "transaction/Transaction.h"
#include <vector>

class Account {
private:
    std::string iban;
    std::string currency;
    AccountStatus status;

    bool isValidIban(const std::string& iban);
    bool isValidCurrency(const std::string& currency);

protected:
    double balance;
    std::vector<Transaction> transactions;
public:
    Account(const std::string& iban, const std::string& currency, double balance);
    virtual ~Account() = default;

    Account(const Account& other);

    virtual void applyInterestOrFee() = 0;
    virtual Account* clone() const = 0;

    std::string getIban() const noexcept;
    std::string getCurrency() const noexcept;
    double getBalance() const noexcept;
    AccountStatus getStatus() const noexcept;
    const std::vector<Transaction>& getTransactions() const noexcept;

    void deposit(double amount, const std::string& description = "");
    void withdraw(double amount, const std::string& description = "");
    void transfer(Account& other, double amount, const std::string& description = "");
    void lock();
    void close();

    std::vector<Transaction> getTransactionsByType(TransactionType type) const;
    std::vector<Transaction> getTransactionsByDateRange(std::time_t from, std::time_t to) const;
    std::vector<Transaction> getTransactionsByMinAmount(double min) const;
    void printMonthlyStatement(int month, int year) const;

    std::map<Category, double> getCategoryReport(std::time_t from, std::time_t to) const;
    void printCategoryReport(std::time_t from, std::time_t to) const;
};