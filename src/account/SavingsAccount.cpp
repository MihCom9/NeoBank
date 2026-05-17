#include "account/SavingsAccount.h"
#include <stdexcept>

SavingsAccount::SavingsAccount(const std::string& iban, const std::string& currency,
                                double balance, double interestRate, int compoundingPeriod)
    : Account(iban, currency, balance), interestRate(interestRate), compoundingPeriod(compoundingPeriod) {
    if (interestRate < 0)
        throw std::invalid_argument("Interest rate cannot be negative");
    if (compoundingPeriod <= 0)
        throw std::invalid_argument("Compounding period must be positive");
}

SavingsAccount::SavingsAccount(const SavingsAccount& other)
    : Account(other), interestRate(other.interestRate), compoundingPeriod(other.compoundingPeriod) {}

Account* SavingsAccount::clone() const {
    return new SavingsAccount(*this);
}

std::ostream& operator<<(std::ostream& out, const SavingsAccount& acc) {
    out << "Type: SavingsAccount\n"
        << "IBAN: " << acc.getIban() << "\n"
        << "Currency: " << acc.getCurrency() << "\n"
        << "Balance: " << acc.getBalance() << "\n"
        << "Interest rate: " << acc.interestRate << "%\n"
        << "Compounding period: " << acc.compoundingPeriod << " months\n";
    return out;
}

std::istream& operator>>(std::istream& in, SavingsAccount& acc) {
    double rate;
    std::cout << "New interest rate: ";
    in >> rate;
    acc.setInterestRate(rate);
    return in;
}

double SavingsAccount::getInterestRate() const noexcept { return interestRate; }
int SavingsAccount::getCompoundingPeriod() const noexcept { return compoundingPeriod; }

void SavingsAccount::setInterestRate(double rate) {
    if (rate < 0)
        throw std::invalid_argument("Interest rate cannot be negative");
    interestRate = rate;
}

void SavingsAccount::applyInterestOrFee() {
    balance += balance * (interestRate / 100.0);
}