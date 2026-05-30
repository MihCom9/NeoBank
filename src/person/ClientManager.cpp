#include "person/ClientManager.h"
#include <iostream>

ClientManager::ClientManager() : nextId(1) {}

ClientManager::~ClientManager() {
    for (Person* p : clients)
        delete p;
}

IndividualClient* ClientManager::registerIndividual(const std::string& name,
                                                     const std::string& email,
                                                     const std::string& phone,
                                                     const std::string& egn,
                                                     const std::string& dateOfBirth,
                                                     const std::string& address) {
    for (Person* p : clients) {
        IndividualClient* ind = dynamic_cast<IndividualClient*>(p);
        if (ind && ind->getEgn() == egn)
            throw std::runtime_error("Client with this EGN already exists");
    }
    IndividualClient* client = new IndividualClient(nextId++, name, email, phone, egn, dateOfBirth, address);
    clients.push_back(client);
    return client;
}

CorporateClient* ClientManager::registerCorporate(const std::string& name,
                                                    const std::string& email,
                                                    const std::string& phone,
                                                    const std::string& eik,
                                                    const std::string& companyName,
                                                    const std::string& vatNumber,
                                                    const std::string& representative) {
    for (Person* p : clients) {
        CorporateClient* corp = dynamic_cast<CorporateClient*>(p);
        if (corp && corp->getEik() == eik)
            throw std::runtime_error("Client with this EIK already exists");
    }
    CorporateClient* client = new CorporateClient(nextId++, name, email, phone, eik, companyName, vatNumber, representative);
    clients.push_back(client);
    return client;
}

const std::vector<Person*>& ClientManager::getAll() const noexcept { return clients; }

Person* ClientManager::findById(int id) const {
    for (Person* p : clients)
        if (p->getId() == id) return p;
    return nullptr;
}

void ClientManager::listAll() const {
    if (clients.empty()) {
        std::cout << "No registered clients.\n";
        return;
    }
    for (Person* p : clients) {
        std::cout << "-------------------------\n";
        std::cout << "ID: " << p->getId() << "\n";
        std::cout << "Name: " << p->getName() << "\n";
        std::cout << "Type: " << p->getClientType() << "\n";
        std::cout << "Email: " << p->getEmail() << "\n";
        std::cout << "Phone: " << p->getPhone() << "\n";
    }
    std::cout << "-------------------------\n";
}