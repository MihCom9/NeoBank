#include "person/IndividualClient.h"
#include <stdexcept>
#include <iostream>

bool IndividualClient::isDigitsOnly(const std::string& s, size_t len) const {
    if (s.size() != len) return false;
    for (char c : s)
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

int IndividualClient::calcAge() const {
    if (dateOfBirth.size() < 10) return -1;
    int birthYear  = std::stoi(dateOfBirth.substr(0, 4));
    int birthMonth = std::stoi(dateOfBirth.substr(5, 2));
    int birthDay   = std::stoi(dateOfBirth.substr(8, 2));

    std::time_t now = std::time(nullptr);
    std::tm* t = std::localtime(&now);
    int curYear  = t->tm_year + 1900;
    int curMonth = t->tm_mon  + 1;
    int curDay   = t->tm_mday;

    int age = curYear - birthYear;
    if (curMonth < birthMonth || (curMonth == birthMonth && curDay < birthDay))
        --age;
    return age;
}

IndividualClient::IndividualClient(const int id, 
                const std::string& name, 
                const std::string& email, 
                const std::string& phone, 
                const std::string& egn, 
                const std::string& dateOfBirth, 
                const std::string& address)
    : Person(id, name, email, phone), egn(egn), 
    dateOfBirth(dateOfBirth), address(address), active(true)
{
    if(!validate() || address.empty()){
        throw std::invalid_argument("Invalid Arguments for Individual Client");
    }
}

std::string IndividualClient::getEgn() const noexcept{return egn;}
std::string IndividualClient::getDateOfBirth() const noexcept{return dateOfBirth;}
std::string IndividualClient::getAddress() const noexcept{return address;}
int IndividualClient::getAge() const{return calcAge();}
bool IndividualClient::isActive() const noexcept{return active;}

void IndividualClient::setAddress(const std::string& address){
    if(address.empty()){
        throw std::invalid_argument("Invalid address argument");
    }
    this->address = address;
}

void IndividualClient::activate(){
    if(active){
        throw std::runtime_error("Account is already active");
    }
    active = true;
}
void IndividualClient::deactivate(){
    if(!active){
        throw std::runtime_error("Account is already inactive");
    }
    active = false;
}

bool IndividualClient::validate() const {
    // 1. ЕГН трябва да е точно 10 цифри
    if (!isDigitsOnly(egn, 10)) return false;
    // 2. Клиентът трябва да е навършил 18 години
    if (calcAge() < 18) return false;
    return true;
}

std::string IndividualClient::getClientType() const {
    return "IndividualClient";
}

void IndividualClient::printInfo() const{
    Person::printInfo();
    std::cout << "ЕГН: "     << egn         << "\n"
                << "Рождена дата: " << dateOfBirth << "\n"
                << "Възраст: " << getAge()    << "\n"
                << "Адрес: "   << address     << "\n"
                << "Статус: "  << (active ? "Активен" : "Деактивиран") << "\n";

}

std::ostream& operator<<(std::ostream& out, const IndividualClient& client) {
    out << static_cast<const Person&>(client)
        << "EGN: " << client.egn << "\n"
        << "Date of birth: " << client.dateOfBirth << "\n"
        << "Age: " << client.getAge() << "\n"
        << "Address: " << client.address << "\n"
        << "Status: " << (client.active ? "Active" : "Inactive") << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, IndividualClient& client) {
    std::string address;
    in >> address;
    client.setAddress(address);
    return in;
}