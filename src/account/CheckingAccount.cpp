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
    const double MONTHLY_FEE = 2.0;
    if (balance < MONTHLY_FEE && !overdraftAllowed) {
        notifications.push_back(Notification(
            "Could not apply monthly fee: insufficient funds",
            NotificationType::GENERAL
        ));
        return;
    }
    balance -= MONTHLY_FEE;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::WITHDRAW,
        MONTHLY_FEE, getCurrency(), getIban(), "", TransactionStatus::SUCCESSFUL,
        "Monthly maintenance fee"
    ));
    notifications.push_back(Notification(
        "Monthly maintenance fee of " + std::to_string(MONTHLY_FEE) + " " + getCurrency() + " applied",
        NotificationType::GENERAL
    ));
}