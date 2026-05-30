#pragma once
#include "account/Account.h"
#include <iostream>

class CreditAccount : public Account {
private:
    double creditLimit;
    double penaltyRate;
    double usedCredit;

public:
    CreditAccount(const std::string& iban, const std::string& currency,
                  double balance, double creditLimit, double penaltyRate);
    
    CreditAccount(const CreditAccount& other);
    CreditAccount& operator=(const CreditAccount& other);

    Account* clone() const override;


    double getCreditLimit() const noexcept;
    double getUsedCredit() const noexcept;
    double getAvailableCredit() const noexcept;

    void withdraw(double amount, const std::string& description = "") override;
    void applyInterestOrFee() override;
    friend std::ostream& operator<<(std::ostream& out, const CreditAccount& acc);
    friend std::istream& operator>>(std::istream& in, CreditAccount& acc);
};