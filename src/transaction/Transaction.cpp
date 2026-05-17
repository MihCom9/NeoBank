#include "transaction/Transaction.h"

Transaction::Transaction(const std::string& id, const TransactionType type, const double amount,
                         const std::string& currency, const std::string& sourceIban,
                         const std::string& targetIban, TransactionStatus status)
    : id(id), type(type), amount(amount), currency(currency),
      sourceIban(sourceIban), targetIban(targetIban),
      timestamp(std::time(nullptr)), status(status) {
    if (id.empty())
        throw std::invalid_argument("Transaction id cannot be empty");
    if (amount <= 0)
        throw std::invalid_argument("Transaction amount must be positive");
    if (currency.empty())
        throw std::invalid_argument("Transaction currency cannot be empty");
    if (sourceIban.empty())
        throw std::invalid_argument("Source IBAN cannot be empty");
}

std::string Transaction::getId() const noexcept { return id; }
TransactionType Transaction::getType() const noexcept { return type; }
double Transaction::getAmount() const noexcept { return amount; }
std::string Transaction::getCurrency() const noexcept { return currency; }
std::string Transaction::getSourceIban() const noexcept { return sourceIban; }
std::string Transaction::getTargetIban() const noexcept { return targetIban; }
TransactionStatus Transaction::getStatus() const noexcept { return status; }
std::time_t Transaction::getTimestamp() const noexcept { return timestamp; }

std::ostream& operator<<(std::ostream& out, const Transaction& t) {
    out << "Transaction ID: " << t.id << "\n"
        << "Type: "   << (t.type == TransactionType::DEPOSIT ? "DEPOSIT" :
                          t.type == TransactionType::WITHDRAW ? "WITHDRAW" : "TRANSFER") << "\n"
        << "Amount: " << t.amount << " " << t.currency << "\n"
        << "From: "   << t.sourceIban << "\n"
        << "To: "     << (t.targetIban.empty() ? "N/A" : t.targetIban) << "\n"
        << "Status: " << (t.status == TransactionStatus::SUCCESSFUL ? "SUCCESSFUL" : "FAILED") << "\n"
        << "Time: "   << std::ctime(&t.timestamp);
    return out;

}