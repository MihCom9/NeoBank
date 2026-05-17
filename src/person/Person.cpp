#include "Person.h"

bool Person::isValidEmail(const std::string& email) {
    size_t atPos = email.find('@');
    if (atPos == std::string::npos) return false;
    size_t dotPos = email.find('.', atPos);
    if (dotPos == std::string::npos) return false;
    return true;
}

bool Person::isValidPhone(const std::string& phone) {
    if (phone.size() < 7) return false;
    for (char c : phone) {
        if (!isdigit(c) && c != ' ' && c != '+' && c != '-')
            return false;
    }
    return true;
}

Person::Person(const int id, const std::string& name, const std::string& email, const std::string& phone)
    : id(id), name(name), email(email), phone(phone) {
    if (id < 0 || name.empty())
        throw std::invalid_argument("Invalid Person constructor argument");
    if (!isValidEmail(email))
        throw std::invalid_argument("Invalid email");
    if (!isValidPhone(phone))
        throw std::invalid_argument("Invalid phone number");
}

int Person::getId() const noexcept { return id; }
std::string Person::getName() const noexcept { return name; }
std::string Person::getEmail() const noexcept { return email; }
std::string Person::getPhone() const noexcept { return phone; }

void Person::setEmail(const std::string& email) {
    if (!isValidEmail(email))
        throw std::invalid_argument("Invalid email");
    this->email = email;
}

void Person::setName(const std::string& name) {
    if (name.empty())
        throw std::invalid_argument("Invalid name");
    this->name = name;
}

void Person::setPhone(const std::string& phone) {
    if (!isValidPhone(phone))
        throw std::invalid_argument("Invalid phone number");
    this->phone = phone;
}