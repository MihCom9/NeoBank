#pragma once
#include <string>
#include "Person.h"
#include <iostream>

class CorporateClient : public Person {
private:
    std::string eik;
    std::string companyName;
    std::string vatNumber;
    std::string representative;
    bool active;

    bool isDigitsOnly(const std::string& s, size_t len) const;

public:
    CorporateClient(int id,
                    const std::string& name,
                    const std::string& email,
                    const std::string& phone,
                    const std::string& eik,
                    const std::string& companyName,
                    const std::string& vatNumber,
                    const std::string& representative);
    ~CorporateClient() = default;
    CorporateClient(const CorporateClient& other);
    CorporateClient& operator=(const CorporateClient& other);

    std::string getEik() const noexcept;
    std::string getCompanyName() const noexcept;
    std::string getVatNumber() const noexcept;
    std::string getRepresentative() const noexcept;
    bool isActive() const noexcept;
    bool isVatRegistered() const noexcept;

    void setCompanyName(const std::string& name);
    void setRepresentative(const std::string& rep);

    void activate();
    void deactivate();

    bool validate() const override;
    std::string getClientType() const override;

    friend std::ostream& operator<<(std::ostream& out, const CorporateClient& client);
    friend std::istream& operator>>(std::istream& in, CorporateClient& client);
};