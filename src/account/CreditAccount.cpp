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

void CreditAccount::withdraw(double amount, const std::string& description) {
    if (getStatus() != AccountStatus::ACTIVE)
        throw std::runtime_error("Account is not active");
    if (amount <= 0)
        throw std::invalid_argument("Withdraw amount must be positive");

    double totalAvailable = balance + getAvailableCredit();
    if (amount > totalAvailable)
        throw std::runtime_error("Insufficient funds and credit");

    if (!limitsettings.checkLimit(amount)) {
        notifications.push_back(Notification(
            "Transaction exceeds limit", NotificationType::LIMIT_REACHED));
        throw std::runtime_error("Transaction exceeds limit");
    }
    if (balance - amount < limitsettings.getMinBalance()) {
        notifications.push_back(Notification(
            "Balance will fall below minimum alert", NotificationType::LOW_BALANCE));
    }

    if (amount > balance) {
        usedCredit += amount - balance;
        balance = 0;
    } else {
        balance -= amount;
    }
    limitsettings.addSpending(amount);
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::WITHDRAW,
        amount, getCurrency(), getIban(), "", TransactionStatus::SUCCESSFUL, description
    ));
}

void CreditAccount::applyInterestOrFee() {
    if (usedCredit <= 0) return;
    double penalty = usedCredit * (penaltyRate / 100.0);
    usedCredit += penalty;
    notifications.push_back(Notification(
        "Penalty interest of " + std::to_string(penalty) + " " + getCurrency() + " applied on used credit",
        NotificationType::LIMIT_REACHED
    ));
}