#include <iostream>
#include "person/ClientManager.h"

int main() {
    ClientManager manager;

    // Register individual
    IndividualClient* ivan = manager.registerIndividual(
        "Ivan Petrov", "ivan@example.com", "+359888123456",
        "9001011234", "1990-01-01", "Sofia, Bulgaria"
    );
    std::cout << *ivan << "\n";

    // Register corporate
    CorporateClient* firma = manager.registerCorporate(
        "Georgi Ivanov", "office@techbg.com", "+35929123456",
        "123456789", "TechBG EOOD", "BG123456789", "Georgi Ivanov"
    );
    std::cout << *firma << "\n";

    // Edit via setters
    ivan->setEmail("ivan.new@example.com");
    ivan->setAddress("Plovdiv, Bulgaria");
    std::cout << "Updated email: " << ivan->getEmail() << "\n";

    // Edit via >>
    std::cin >> *firma;
    std::cout << *firma << "\n";

    // Deactivate
    ivan->deactivate();
    std::cout << "Ivan active: " << (ivan->isActive() ? "yes" : "no") << "\n";

    // Try deactivating again — expect error
    try {
        ivan->deactivate();
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // Invalid registration
    try {
        manager.registerIndividual(
            "Bad Client", "bad@example.com", "+359000000",
            "12345", "2010-01-01", "Nowhere"
        );
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // List all
    manager.listAll();

    return 0;
}