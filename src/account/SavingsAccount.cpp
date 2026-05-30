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
    double periodicRate = interestRate / 100.0 / compoundingPeriod;
    double interest = balance * periodicRate;
    if (interest <= 0) return;
    balance += interest;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::DEPOSIT,
        interest, getCurrency(), getIban(), "", TransactionStatus::SUCCESSFUL,
        "Interest accrual"
    ));
    notifications.push_back(Notification(
        "Interest of " + std::to_string(interest) + " " + getCurrency() + " accrued",
        NotificationType::GENERAL
    ));
}