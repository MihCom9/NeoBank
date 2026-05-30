#pragma once
#include "account/Account.h"
#include <iostream>

class SavingsAccount : public Account {
private:
    double interestRate;
    int compoundingPeriod;

public:
    SavingsAccount(const std::string& iban, const std::string& currency,
                   double balance, double interestRate, int compoundingPeriod);

    SavingsAccount(const SavingsAccount& other);
    SavingsAccount& operator=(const SavingsAccount& other);

    Account* clone() const override;

    double getInterestRate() const noexcept;
    int getCompoundingPeriod() const noexcept;
    void setInterestRate(double rate);

    void applyInterestOrFee() override;


    friend std::ostream& operator<<(std::ostream& out, const SavingsAccount& acc);
    friend std::istream& operator>>(std::istream& in, SavingsAccount& acc);
};