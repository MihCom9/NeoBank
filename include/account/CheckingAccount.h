#pragma once
#include<iostream>
#include "account/Account.h"

class CheckingAccount : public Account {
private:
    double dailyLimit;
    bool overdraftAllowed;

public:
    CheckingAccount(const std::string& iban, const std::string& currency,
                    double balance, double dailyLimit, bool overdraftAllowed);
    CheckingAccount(const CheckingAccount& other);

    Account* clone() const override;
    
    double getDailyLimit() const noexcept;
    bool isOverdraftAllowed() const noexcept;
    void setDailyLimit(double limit);

    void applyInterestOrFee() override;

    friend std::ostream& operator<<(std::ostream& out, const CheckingAccount& acc);
    friend std::istream& operator>>(std::istream& in, CheckingAccount& acc);
};