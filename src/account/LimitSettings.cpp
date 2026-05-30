#include "account/LimitSettings.h"
#include<ctime>
LimitSettings::LimitSettings()
    : dailyLimit(1e9), singleTxLimit(1e9), minBalance(0), dailySpent(0),
      lastResetDate(std::time(nullptr)) {}

LimitSettings::LimitSettings(const double dailyLimit, const double singleTxLimit,
         const double minBalance): dailyLimit(dailyLimit), 
         singleTxLimit(singleTxLimit), minBalance(minBalance), dailySpent(0),
         lastResetDate(std::time(nullptr)) 
         {}

double LimitSettings::getDailyLimit() const{return dailyLimit;}
double LimitSettings::getSingleTxLimit() const{return singleTxLimit;}
double LimitSettings::getMinBalance() const{return minBalance;}
double LimitSettings::getDailySpent() const{return dailySpent;}

bool LimitSettings::checkLimit(double amount){
    resetIfNewDay();
    if(amount > singleTxLimit || dailySpent + amount > dailyLimit){
        return false;
    }
    return true;
}

void LimitSettings::setDailyLimit(double dailyLimit){
    this->dailyLimit = dailyLimit;
}
void LimitSettings::setMinBalance(double minBalance){
    this->minBalance = minBalance;
}
void LimitSettings::setSingleTxLimit(double singleTxLimit){
    this->singleTxLimit = singleTxLimit;
}
void LimitSettings::addSpending(double amount){
    dailySpent+=amount;
}
void LimitSettings::resetIfNewDay(){
    std::time_t now = std::time(nullptr);
    std::tm* lastReset = std::localtime(&lastResetDate);
    int lastDay = lastReset->tm_yday;
    int lastYear = lastReset->tm_year;

    std::tm* today = std::localtime(&now);
    if (today->tm_yday != lastDay || today->tm_year != lastYear) {
        dailySpent = 0;
        lastResetDate = now;
    }
}