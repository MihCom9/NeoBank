#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "../notification/Notification.h"
#include "../account/Account.h"

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
    std::vector<Notification> notifications;

public:
    Person(const int id, const std::string& name, const std::string& email, const std::string& phone);
    virtual ~Person() = default;

    virtual std::string getClientType() const = 0;
    virtual bool validate() const = 0;

    int getId() const noexcept;
    std::string getName() const noexcept;
    std::string getEmail() const noexcept;
    std::string getPhone() const noexcept;

    void setEmail(const std::string& email);
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
};