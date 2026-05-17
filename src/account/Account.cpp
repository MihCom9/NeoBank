#include "account/Account.h"

bool Account::isValidIban(const std::string& iban) {
    return iban.size() >= 15 && iban.size() <= 34
        && std::isalpha(iban[0]) && std::isalpha(iban[1])
        && std::isdigit(iban[2]) && std::isdigit(iban[3]);
}

bool Account::isValidCurrency(const std::string& currency) {
    return currency.size() == 3
        && std::isupper(currency[0])
        && std::isupper(currency[1])
        && std::isupper(currency[2]);
}

Account::Account(const std::string& iban, const std::string& currency, double balance)
    : iban(iban), currency(currency), balance(balance), status(AccountStatus::ACTIVE), transactions() {
    if (!isValidIban(iban))
        throw std::invalid_argument("Invalid IBAN");
    if (!isValidCurrency(currency))
        throw std::invalid_argument("Invalid currency code");
    if (balance < 0)
        throw std::invalid_argument("Balance cannot be negative");
}

Account::Account(const Account& other)
    : iban(other.iban), currency(other.currency),
      balance(other.balance), status(other.status),
      transactions(other.transactions) {
}

std::string Account::getIban() const noexcept { return iban; }
std::string Account::getCurrency() const noexcept { return currency; }
double Account::getBalance() const noexcept { return balance; }
AccountStatus Account::getStatus() const noexcept { return status; }
const std::vector<Transaction>& Account::getTransactions() const noexcept{return transactions;}

void Account::deposit(double amount) {
    if (status != AccountStatus::ACTIVE)
        throw std::runtime_error("Account is not active");
    if (amount <= 0)
        throw std::invalid_argument("Deposit amount must be positive");
    balance += amount;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::DEPOSIT,
        amount, currency, iban, "", TransactionStatus::SUCCESSFUL
    ));
}

void Account::withdraw(double amount) {
    if (status != AccountStatus::ACTIVE)
        throw std::runtime_error("Account is not active");
    if (amount <= 0)
        throw std::invalid_argument("Withdraw amount must be positive");
    if (amount > balance)
        throw std::runtime_error("Insufficient funds");
    balance -= amount;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::WITHDRAW,
        amount, currency, iban, "", TransactionStatus::SUCCESSFUL
    ));
}

void Account::transfer(Account& other, double amount) {
    if (status != AccountStatus::ACTIVE)
        throw std::runtime_error("Account is not active");
    if (amount <= 0)
        throw std::invalid_argument("Transfer amount must be positive");
    if (amount > balance)
        throw std::runtime_error("Insufficient funds");
    balance -= amount;
    other.balance += amount;
    transactions.push_back(Transaction(
        std::to_string(transactions.size() + 1),
        TransactionType::TRANSFER,
        amount, currency, iban, other.iban, TransactionStatus::SUCCESSFUL
    ));
    other.transactions.push_back(Transaction(
        std::to_string(other.transactions.size() + 1),
        TransactionType::TRANSFER,
        amount, currency, iban, other.iban, TransactionStatus::SUCCESSFUL
    ));
}

void Account::lock() {
    if (status == AccountStatus::CLOSED)
        throw std::runtime_error("Cannot lock a closed account");
    status = AccountStatus::LOCKED;
}

void Account::close() {
    if (status == AccountStatus::CLOSED)
        throw std::runtime_error("Account is already closed");
    status = AccountStatus::CLOSED;
}