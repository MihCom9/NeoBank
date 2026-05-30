#include "transaction/Transaction.h"
#include <algorithm>
#include <cctype>

static std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

Category Transaction::categorize(const std::string& description) {
    std::string d = toLower(description);
    auto contains = [&](const std::string& kw) {
        return d.find(kw) != std::string::npos;
    };

    if (contains("lidl") || contains("kaufland") || contains("billa") ||
        contains("food") || contains("restaurant") || contains("lunch") ||
        contains("dinner") || contains("cafe") || contains("grocery") ||
        contains("supermarket") || contains("market"))
        return Category::FOOD;

    if (contains("uber") || contains("taxi") || contains("bus") ||
        contains("metro") || contains("fuel") || contains("gas") ||
        contains("parking") || contains("train") || contains("transport"))
        return Category::TRANSPORT;

    if (contains("electricity") || contains("water") || contains("internet") ||
        contains("phone") || contains("rent") || contains("bill") ||
        contains("utility") || contains("utilities"))
        return Category::UTILITIES;

    if (contains("netflix") || contains("cinema") || contains("spotify") ||
        contains("game") || contains("concert") || contains("sport") ||
        contains("entertainment") || contains("ticket"))
        return Category::ENTERTAINMENT;

    if (contains("pharmacy") || contains("doctor") || contains("hospital") ||
        contains("medicine") || contains("dental") || contains("health"))
        return Category::HEALTH;

    if (contains("amazon") || contains("zara") || contains("clothes") ||
        contains("shop") || contains("store") || contains("online") ||
        contains("purchase"))
        return Category::SHOPPING;

    return Category::OTHER;
}

Transaction::Transaction(const std::string& id, const TransactionType type, const double amount,
                         const std::string& currency, const std::string& sourceIban,
                         const std::string& targetIban, TransactionStatus status,
                         const std::string& description)
    : id(id), type(type), amount(amount), currency(currency),
      sourceIban(sourceIban), targetIban(targetIban),
      timestamp(std::time(nullptr)), status(status),
      category(categorize(description)), description(description) {
    if (id.empty())
        throw std::invalid_argument("Transaction id cannot be empty");
    if (amount <= 0)
        throw std::invalid_argument("Transaction amount must be positive");
    if (currency.empty())
        throw std::invalid_argument("Transaction currency cannot be empty");
    if (sourceIban.empty())
        throw std::invalid_argument("Source IBAN cannot be empty");
}

static std::string categoryName(Category c) {
    switch (c) {
        case Category::FOOD:          return "FOOD";
        case Category::TRANSPORT:     return "TRANSPORT";
        case Category::UTILITIES:     return "UTILITIES";
        case Category::ENTERTAINMENT: return "ENTERTAINMENT";
        case Category::HEALTH:        return "HEALTH";
        case Category::SHOPPING:      return "SHOPPING";
        default:                      return "OTHER";
    }
}

std::string Transaction::getId() const noexcept { return id; }
TransactionType Transaction::getType() const noexcept { return type; }
double Transaction::getAmount() const noexcept { return amount; }
std::string Transaction::getCurrency() const noexcept { return currency; }
std::string Transaction::getSourceIban() const noexcept { return sourceIban; }
std::string Transaction::getTargetIban() const noexcept { return targetIban; }
TransactionStatus Transaction::getStatus() const noexcept { return status; }
std::time_t Transaction::getTimestamp() const noexcept { return timestamp; }
Category Transaction::getCategory() const noexcept { return category; }
std::string Transaction::getDescription() const noexcept { return description; }

std::ostream& operator<<(std::ostream& out, const Transaction& t) {
    out << "Transaction ID: " << t.id << "\n"
        << "Type: "     << (t.type == TransactionType::DEPOSIT ? "DEPOSIT" :
                            t.type == TransactionType::WITHDRAW ? "WITHDRAW" : "TRANSFER") << "\n"
        << "Amount: "   << t.amount << " " << t.currency << "\n"
        << "From: "     << t.sourceIban << "\n"
        << "To: "       << (t.targetIban.empty() ? "N/A" : t.targetIban) << "\n"
        << "Category: " << categoryName(t.category) << "\n"
        << "Desc: "     << (t.description.empty() ? "—" : t.description) << "\n"
        << "Status: "   << (t.status == TransactionStatus::SUCCESSFUL ? "SUCCESSFUL" : "FAILED") << "\n"
        << "Time: "     << std::ctime(&t.timestamp);
    return out;
}