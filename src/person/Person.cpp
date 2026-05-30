#include "person/Person.h"
#include<iostream>

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
    : id(id), name(name), email(email), phone(phone), accounts() {
    if (id <= 0 || name.empty())
        throw std::invalid_argument("Invalid Person constructor argument");
    if (!isValidEmail(email))
        throw std::invalid_argument("Invalid email");
    if (!isValidPhone(phone))
        throw std::invalid_argument("Invalid phone number");
}

Person::Person(const Person& other)
    : id(other.id), name(other.name),
      email(other.email), phone(other.phone),
      loans(other.loans), notifications(other.notifications) {
    for (Account* a : other.accounts)
        accounts.push_back(a->clone());
}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        for (Account* a : accounts) delete a;
        accounts.clear();
        id = other.id;
        name = other.name;
        email = other.email;
        phone = other.phone;
        loans = other.loans;
        notifications = other.notifications;
        for (Account* a : other.accounts)
            accounts.push_back(a->clone());
    }
    return *this;
}

Person::~Person() {
    for (Account* a : accounts)
        delete a;
}

void Person::printInfo() const {
    std::cout << "ID: "       << id    << "\n"
            << "Тип: "     << getClientType() << "\n"
            << "Име: "     << name  << "\n"
            << "Email: "   << email << "\n"
            << "Телефон: " << phone << "\n";
}

int Person::getId() const noexcept { return id; }
std::string Person::getName() const noexcept { return name; }
std::string Person::getEmail() const noexcept { return email; }
std::string Person::getPhone() const noexcept { return phone; }
const std::vector<Notification>& Person::getNotifications() const noexcept{return notifications;}
const std::vector<Account*>& Person::getAccounts() const noexcept {
    return accounts;
}
std::vector<Loan>& Person::getLoans() noexcept { return loans; }
const std::vector<Loan>& Person::getLoans() const noexcept { return loans; }

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

void Person::addNotification(const Notification& notification){
    notifications.push_back(notification);
}

void Person::openAccount(Account* account){
    if (!account)
        throw std::invalid_argument("Account cannot be null");
    accounts.push_back(account->clone());
}

void Person::applyForLoan(double principal, double rate, int months){
    loans.push_back(Loan(principal,rate, months));
}

std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << "ID: " << p.id << "\n"
        << "Name: " << p.name << "\n"
        << "Email: " << p.email << "\n"
        << "Phone: " << p.phone << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, Person& p) {
    std::string name, email, phone;
    std::cout << "New name: ";
    std::getline(in >> std::ws, name);
    std::cout << "New email: ";
    std::getline(in >> std::ws, email);
    std::cout << "New phone: ";
    std::getline(in >> std::ws, phone);
    p.setName(name);
    p.setEmail(email);
    p.setPhone(phone);
    return in;
}