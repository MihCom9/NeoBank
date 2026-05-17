#pragma once
#include<string>
#include<stdexcept>
#include<iostream>
#include "enums/TransactionStatus.h"
#include "enums/TransactionType.h"
#include <ctime>

class Transaction
{
private:
    std::string id;
    TransactionType type;
    double amount;
    std::string currency;
    std::string sourceIban;
    std::string targetIban;
    std::time_t timestamp;
    TransactionStatus status;
public:
    Transaction(const std::string& id, const TransactionType type, const double amount, 
            const std::string& currency, const std::string& sourceIban, const std::string& targetIban, TransactionStatus status);
    ~Transaction(){}

    std::string getId() const noexcept;
    TransactionType getType() const noexcept;
    double getAmount() const noexcept;
    std::string getCurrency() const noexcept;
    std::string getSourceIban() const noexcept;
    std::string getTargetIban() const noexcept;
    TransactionStatus getStatus() const noexcept;
    std::time_t getTimestamp() const noexcept;

    friend std::ostream& operator<<(std::ostream& out,const Transaction& transaction);
};
