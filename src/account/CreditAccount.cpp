#include "account/CreditAccount.h"
#include <stdexcept>

CreditAccount::CreditAccount(const std::string& iban, const std::string& currency,
                              double balance, double creditLimit, double penaltyRate)
    : Account(iban, currency, balance), creditLimit(creditLimit),
      penaltyRate(penaltyRate), usedCredit(0) {
    if (creditLimit <= 0)
        throw std::invalid_argument("Credit limit must be positive");
    if (penaltyRate < 0)
        throw std::invalid_argument("Penalty rate cannot be negative");
}

CreditAccount::CreditAccount(const CreditAccount& other)
    : Account(other), creditLimit(other.creditLimit),
      penaltyRate(other.penaltyRate), usedCredit(other.usedCredit) {}

Account* CreditAccount::clone() const {
    return new CreditAccount(*this);
}

std::ostream& operator<<(std::ostream& out, const CreditAccount& acc) {
    out << "Type: CreditAccount\n"
        << "IBAN: " << acc.getIban() << "\n"
        << "Currency: " << acc.getCurrency() << "\n"
        << "Balance: " << acc.getBalance() << "\n"
        << "Credit limit: " << acc.creditLimit << "\n"
        << "Used credit: " << acc.usedCredit << "\n"
        << "Available credit: " << acc.getAvailableCredit() << "\n"
        << "Penalty rate: " << acc.penaltyRate << "%\n";
    return out;
}

std::istream& operator>>(std::istream& in, CreditAccount& acc) {
    double limit;
    std::cout << "New credit limit: ";
    in >> limit;
    if (limit <= 0)
        throw std::invalid_argument("Credit limit must be positive");
    acc.creditLimit = limit;
    return in;
}

double CreditAccount::getCreditLimit() const noexcept { return creditLimit; }
double CreditAccount::getUsedCredit() const noexcept { return usedCredit; }
double CreditAccount::getAvailableCredit() const noexcept { return creditLimit - usedCredit; }

void CreditAccount::applyInterestOrFee() {
    if (usedCredit > 0)
        usedCredit += usedCredit * (penaltyRate / 100.0);
}