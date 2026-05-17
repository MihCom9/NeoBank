#pragma once
#include <string>
#include <stdexcept>
#include "enums/AccountStatus.h"

class Account {
private:
    std::string iban;
    std::string currency;
    AccountStatus status;

    bool isValidIban(const std::string& iban);
    bool isValidCurrency(const std::string& currency);

protected:
    double balance;

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

    void deposit(double amount);
    void withdraw(double amount);
    void transfer(Account& other, double amount);
    void lock();
    void close();

};