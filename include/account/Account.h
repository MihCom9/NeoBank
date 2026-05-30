#pragma once
#include <string>
#include <stdexcept>
#include <map>
#include "enums/AccountStatus.h"
#include "enums/Category.h"
#include "transaction/Transaction.h"
#include "account/LimitSettings.h"
#include "notification/Notification.h"
#include <vector>

class Account {
private:
    std::string iban;
    std::string currency;
    AccountStatus status;

    bool isValidIban(const std::string& iban);
    bool isValidCurrency(const std::string& currency);

protected:
    Account& operator=(const Account& other);

protected:
    double balance;
    std::vector<Transaction> transactions;
    LimitSettings limitsettings;
    std::vector<Notification> notifications;
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
    const LimitSettings& getLimitSettings() const noexcept;
    const std::vector<Notification>& getNotifications() const noexcept;
    const std::vector<Transaction>& getTransactions() const noexcept;

    virtual void deposit(double amount, const std::string& description = "");
    virtual void withdraw(double amount, const std::string& description = "");
    virtual void transfer(Account& other, double amount, const std::string& description = "");
    void lock();
    void close();
    void setLimitSettings(const LimitSettings& limitSettings);

    std::vector<Transaction> getTransactionsByType(TransactionType type) const;
    std::vector<Transaction> getTransactionsByDateRange(std::time_t from, std::time_t to) const;
    std::vector<Transaction> getTransactionsByMinAmount(double min) const;
    void printMonthlyStatement(int month, int year) const;

    std::map<Category, double> getCategoryReport(std::time_t from, std::time_t to) const;
    void printCategoryReport(std::time_t from, std::time_t to) const;
};