#pragma once
#include<string>
#include "Person.h"


// =============================================================
//  IndividualClient — физическо лице
//  Допълнителни полета: ЕГН, дата на раждане, адрес
//  Валидация: формат на ЕГН (10 цифри) + навършени 18 години
// =============================================================
class IndividualClient : public Person
{
private:
    std::string egn;
    std::string dateOfBirth; // формат: "YYYY-MM-DD"
    std::string address;
    bool active;

    bool isDigitsOnly(const std::string& s, size_t len) const;
 
    // Изчислява възраст в години от "YYYY-MM-DD"
    int calcAge() const;

public:
    IndividualClient(const int id, 
                const std::string& name, 
                const std::string& email, 
                const std::string& phone, 
                const std::string& egn, 
                const std::string& dateOfBirth, 
                const std::string& address);

    ~IndividualClient(){}
    
    std::string getEgn() const noexcept;
    std::string getDateOfBirth() const noexcept;
    std::string getAddress() const noexcept;
    int getAge() const;
    bool isActive() const noexcept;
    
    void setAddress(const std::string& address);

    void activate();
    void deactivate();

    bool validate() const override;
    std::string getClientType() const override;
    void printInfo() const override;


    friend std::ostream& operator<<(std::ostream& out, const IndividualClient& client);
    friend std::istream& operator>>(std::istream& in, IndividualClient& client);
};
