#include "account/CheckingAccount.h"
#include <stdexcept>

CheckingAccount::CheckingAccount(const std::string& iban, const std::string& currency,
                                  double balance, double dailyLimit, bool overdraftAllowed)
    : Account(iban, currency, balance), dailyLimit(dailyLimit), overdraftAllowed(overdraftAllowed) {
    if (dailyLimit <= 0)
        throw std::invalid_argument("Daily limit must be positive");
}

CheckingAccount::CheckingAccount(const CheckingAccount& other)
    : Account(other), dailyLimit(other.dailyLimit), overdraftAllowed(other.overdraftAllowed) {}

Account* CheckingAccount::clone() const {
    return new CheckingAccount(*this);
}

std::ostream& operator<<(std::ostream& out, const CheckingAccount& acc) {
    out << "Type: CheckingAccount\n"
        << "IBAN: " << acc.getIban() << "\n"
        << "Currency: " << acc.getCurrency() << "\n"
        << "Balance: " << acc.getBalance() << "\n"
        << "Daily limit: " << acc.dailyLimit << "\n"
        << "Overdraft allowed: " << (acc.overdraftAllowed ? "yes" : "no") << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, CheckingAccount& acc) {
    double limit;
    std::cout << "New daily limit: ";
    in >> limit;
    acc.setDailyLimit(limit);
    return in;
}

double CheckingAccount::getDailyLimit() const noexcept { return dailyLimit; }
bool CheckingAccount::isOverdraftAllowed() const noexcept { return overdraftAllowed; }

void CheckingAccount::setDailyLimit(double limit) {
    if (limit <= 0)
        throw std::invalid_argument("Daily limit must be positive");
    dailyLimit = limit;
}

void CheckingAccount::applyInterestOrFee() {
    if (getBalance() == 0)
        balance -= 2.0;
}