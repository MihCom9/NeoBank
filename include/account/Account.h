#pragma once
#include<string>
#include "../enums/AccountStatus.h"

class Account
{
private:
    std::string iban;
    std::string currency;
    double balance;
    AccountStatus status;

    bool isValidIban(const std::string& iban);
    bool isValidCurrency(const std::string& currency);
    
public:
    Account(const std::string& iban, const std::string& currency,double balance);
    ~Account(){}
};

