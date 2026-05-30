#pragma once
#include<ctime>

class LimitSettings
{
private:
   double dailyLimit;
   double singleTxLimit;
   double minBalance;
   double dailySpent;
   std::time_t lastResetDate;

public:
    LimitSettings();
    LimitSettings(const double dailyLimit, const double singleTxLimit,
         const double minBalance);
    ~LimitSettings(){}

    double getDailyLimit() const;
    double getSingleTxLimit() const;
    double getMinBalance() const;
    double getDailySpent() const;

    bool checkLimit(double amount);
    void setDailyLimit(double dailyLimit);
    void setMinBalance(double minBalance);
    void setSingleTxLimit(double singletxLimit);
    void addSpending(double amount);
    void resetIfNewDay();
};