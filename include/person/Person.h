#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "../notification/Notification.h"
#include "../account/Account.h"
#include "loan/Loan.h"
#include <ostream> 

class Person {
private:
    int id;
    std::string name;
    std::string email;
    std::string phone;

    bool isValidEmail(const std::string& email);
    bool isValidPhone(const std::string& phone);

protected:
    std::vector<Account*> accounts;
    std::vector<Loan> loans;
    std::vector<Notification> notifications;

public:
    Person(const int id, const std::string& name, const std::string& email, const std::string& phone);
    Person(const Person& other);
    virtual ~Person();

    virtual std::string getClientType() const = 0;
    virtual bool validate() const = 0;
    virtual void printInfo() const;

    int getId() const noexcept;
    std::string getName() const noexcept;
    std::string getEmail() const noexcept;
    std::string getPhone() const noexcept;
    std::vector<Loan>& getLoans() noexcept;
    const std::vector<Loan>& getLoans() const noexcept;
    const std::vector<Notification>& getNotifications() const noexcept;
    const std::vector<Account*>& getAccounts() const noexcept;

    Person& operator=(const Person& other);

    void setEmail(const std::string& email);
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void addNotification(const Notification& notification);
    void openAccount(Account* account);
    void applyForLoan(double principal, double rate, int months);

    friend std::ostream& operator<<(std::ostream& out, const Person& p);
    friend std::istream& operator>>(std::istream& in, Person& p);
};