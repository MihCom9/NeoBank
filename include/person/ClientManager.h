#pragma once
#include <vector>
#include <stdexcept>
#include "IndividualClient.h"
#include "CorporateClient.h"

class ClientManager {
private:
    std::vector<Person*> clients;
    int nextId;

public:
    ClientManager();
    ~ClientManager();
    ClientManager(const ClientManager&) = delete;
    ClientManager& operator=(const ClientManager&) = delete;

    IndividualClient* registerIndividual(const std::string& name,
                                         const std::string& email,
                                         const std::string& phone,
                                         const std::string& egn,
                                         const std::string& dateOfBirth,
                                         const std::string& address);

    CorporateClient* registerCorporate(const std::string& name,
                                        const std::string& email,
                                        const std::string& phone,
                                        const std::string& eik,
                                        const std::string& companyName,
                                        const std::string& vatNumber,
                                        const std::string& representative);

    Person* findById(int id) const;
    void listAll() const;
};