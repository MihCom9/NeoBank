#pragma once
#include<string>
#include<vector>
#include<ctime>
#include <random>
#include "enums/LoanStatus.h"

class Loan
{
private:
    std::string id;
    double principal;
    double interestRate;
    int termMonths;
    double remainingDebt;
    LoanStatus status;
    std::vector<double> payments;
    std::time_t startDate;

public:
    Loan(const double principal, const double interestRate, 
        const unsigned int termMonths):
        id(), principal(principal), interestRate(interestRate),
        termMonths(termMonths),  remainingDebt(principal), status(LoanStatus::ACTIVE), 
        payments(), startDate(std::time(nullptr))
    {
        if(principal < 0 || interestRate < 0){
            throw std::invalid_argument("Arguments are invalid for loan");
        }
        static std::mt19937 gen(std::random_device{}());
        static std::uniform_int_distribution<> dis(1000, 9999);
        id = "LOAN-" + std::to_string(dis(gen));
    }
    Loan(const Loan& other);
    Loan& operator=(const Loan& other);
    ~Loan(){}

    double calculateMonthlyPayment() const;
    void makePayment(double amount);
    void checkOverdue();

    double getRemainingDebt() const;
    bool isOverdue() const;
    LoanStatus getStatus() const;
    std::vector<double> getPaymentSchedule() const;
};