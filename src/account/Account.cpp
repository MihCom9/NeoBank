#include "account/Account.h"

bool Account::isValidIban(const std::string& iban){
    return iban.size() >= 15 && iban.size() <= 34 
        && std::isalpha(iban[0]) && std::isalpha(iban[1])
        && std::isdigit(iban[2]) && std::isdigit(iban[3]);
}

bool Account::isValidCurrency(const std::string& currency){
    return currency.size() == 3 
        && std::isupper(currency[0]) 
        && std::isupper(currency[1]) 
        && std::isupper(currency[2]);
}   

Account::Account(const std::string& iban, const std::string& currency,double balance)
        : iban(iban), currency(currency), balance(balance), status(AccountStatus::ACTIVE){

}