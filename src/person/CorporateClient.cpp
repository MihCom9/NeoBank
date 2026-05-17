#include "person/CorporateClient.h"
#include <stdexcept>

bool CorporateClient::isDigitsOnly(const std::string& s, size_t len) const {
    if (s.size() != len) return false;
    for (char c : s)
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

CorporateClient::CorporateClient(int id,
                                 const std::string& name,
                                 const std::string& email,
                                 const std::string& phone,
                                 const std::string& eik,
                                 const std::string& companyName,
                                 const std::string& vatNumber,
                                 const std::string& representative)
    : Person(id, name, email, phone),
      eik(eik), companyName(companyName),
      vatNumber(vatNumber), representative(representative),
      active(true)
{
    if (!validate() || companyName.empty() || representative.empty())
        throw std::invalid_argument("Invalid Arguments for Corporate Client");
}

std::string CorporateClient::getEik() const noexcept { return eik; }
std::string CorporateClient::getCompanyName() const noexcept { return companyName; }
std::string CorporateClient::getVatNumber() const noexcept { return vatNumber; }
std::string CorporateClient::getRepresentative() const noexcept { return representative; }
bool CorporateClient::isActive() const noexcept { return active; }
bool CorporateClient::isVatRegistered() const noexcept { return !vatNumber.empty(); }

void CorporateClient::setCompanyName(const std::string& name) {
    if (name.empty())
        throw std::invalid_argument("Company name cannot be empty");
    companyName = name;
}

void CorporateClient::setRepresentative(const std::string& rep) {
    if (rep.empty())
        throw std::invalid_argument("Representative cannot be empty");
    representative = rep;
}

void CorporateClient::activate() {
    if (active)
        throw std::runtime_error("Account is already active");
    active = true;
}

void CorporateClient::deactivate() {
    if (!active)
        throw std::runtime_error("Account is already inactive");
    active = false;
}

bool CorporateClient::validate() const {
    return isDigitsOnly(eik, 9) || isDigitsOnly(eik, 13);
}

std::string CorporateClient::getClientType() const {
    return "CorporateClient";
}

std::ostream& operator<<(std::ostream& out, const CorporateClient& client) {
    out << static_cast<const Person&>(client)
        << "EIK: " << client.eik << "\n"
        << "Company: " << client.companyName << "\n"
        << "VAT: " << (client.isVatRegistered() ? client.vatNumber : "Not registered") << "\n"
        << "Representative: " << client.representative << "\n"
        << "Status: " << (client.active ? "Active" : "Inactive") << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, CorporateClient& client) {
    std::string companyName, representative;
    std::cout << "New company name: ";
    if (in.peek() == '\n') in.ignore();
    std::getline(in, companyName);
    std::cout << "New representative: ";
    std::getline(in, representative);
    client.setCompanyName(companyName);
    client.setRepresentative(representative);
    return in;
}