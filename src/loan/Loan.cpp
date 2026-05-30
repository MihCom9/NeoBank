#include "loan/Loan.h"
#include <cmath>

Loan::Loan(const Loan& other)
    : id(other.id), principal(other.principal), interestRate(other.interestRate),
      termMonths(other.termMonths), remainingDebt(other.remainingDebt),
      status(other.status), payments(other.payments), startDate(other.startDate) {}

Loan& Loan::operator=(const Loan& other) {
    if (this != &other) {
        id = other.id;
        principal = other.principal;
        interestRate = other.interestRate;
        termMonths = other.termMonths;
        remainingDebt = other.remainingDebt;
        status = other.status;
        payments = other.payments;
        startDate = other.startDate;
    }
    return *this;
}

double Loan::calculateMonthlyPayment() const {
    double r = interestRate / 12.0 / 100.0;
    if (r == 0.0) return principal / termMonths;
    double n = termMonths;
    return principal * r * std::pow(1 + r, n) / (std::pow(1 + r, n) - 1);
}

void Loan::makePayment(double amount) {
    if (amount <= 0) {
        throw std::invalid_argument("Payment amount must be positive");
    }
    remainingDebt -= amount;
    payments.push_back(amount);
    if (remainingDebt <= 0) {
        remainingDebt = 0;
        status = LoanStatus::PAID;
    }
}

double Loan::getRemainingDebt() const {
    return remainingDebt;
}

void Loan::checkOverdue() {
    if (status != LoanStatus::ACTIVE) return;
    double secondsElapsed = std::difftime(std::time(nullptr), startDate);
    int monthsElapsed = static_cast<int>(secondsElapsed / (30.44 * 24 * 3600));
    if (monthsElapsed > static_cast<int>(payments.size()))
        status = LoanStatus::OVERDUE;
}

bool Loan::isOverdue() const {
    return status == LoanStatus::OVERDUE;
}

LoanStatus Loan::getStatus() const {
    return status;
}

std::vector<double> Loan::getPaymentSchedule() const {
    double monthly = calculateMonthlyPayment();
    return std::vector<double>(termMonths, monthly);
}