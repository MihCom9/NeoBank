#include <iostream>
#include "person/ClientManager.h"
#include "account/CheckingAccount.h"
#include "account/SavingsAccount.h"
#include "account/CreditAccount.h"

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

    // Create and open accounts
    CheckingAccount* ivanAcc = new CheckingAccount("BG80BNBG96611020345678", "BGN", 1000.0, 500.0, false);
    ivan->openAccount(ivanAcc);
    delete ivanAcc;

    SavingsAccount* ivanSavings = new SavingsAccount("BG80BNBG96611020345679", "BGN", 2000.0, 3.5, 12);
    ivan->openAccount(ivanSavings);
    delete ivanSavings;

    CheckingAccount* firmaAcc = new CheckingAccount("BG80BNBG96611020345680", "BGN", 5000.0, 2000.0, true);
    firma->openAccount(firmaAcc);
    delete firmaAcc;

    // Print accounts
    std::cout << *static_cast<CheckingAccount*>(ivan->getAccounts()[0]) << "\n";
    std::cout << *static_cast<SavingsAccount*>(ivan->getAccounts()[1]) << "\n";

    // Deposit
    ivan->getAccounts()[0]->deposit(500.0);
    std::cout << "After deposit: " << ivan->getAccounts()[0]->getBalance() << "\n";

    // Withdraw
    ivan->getAccounts()[0]->withdraw(200.0);
    std::cout << "After withdraw: " << ivan->getAccounts()[0]->getBalance() << "\n";

    // Transfer from ivan to firma
    ivan->getAccounts()[0]->transfer(*firma->getAccounts()[0], 100.0);
    std::cout << "Ivan balance after transfer: " << ivan->getAccounts()[0]->getBalance() << "\n";
    std::cout << "Firma balance after transfer: " << firma->getAccounts()[0]->getBalance() << "\n";

    // Try withdrawing more than balance — expect error
    try {
        ivan->getAccounts()[0]->withdraw(999999.0);
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // Lock account and try to deposit — expect error
    ivan->getAccounts()[0]->lock();
    try {
        ivan->getAccounts()[0]->deposit(100.0);
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // Apply interest on savings
    ivan->getAccounts()[1]->applyInterestOrFee();
    std::cout << "After interest: " << ivan->getAccounts()[1]->getBalance() << "\n";

    std::cout << "\n-- Ivan's account transactions --\n";
    for (const Transaction& t : ivan->getAccounts()[0]->getTransactions())
        std::cout << t << "\n";
    
    // Filter by type
    std::cout << "\n-- Deposits only --\n";
    for (const Transaction& t : ivan->getAccounts()[0]->getTransactionsByType(TransactionType::DEPOSIT))
        std::cout << t << "\n";

    // Filter by min amount
    std::cout << "\n-- Transactions above 300 BGN --\n";
    for (const Transaction& t : ivan->getAccounts()[0]->getTransactionsByMinAmount(300.0))
        std::cout << t << "\n";

    // Filter by date range — from start of today to now
    std::time_t startOfDay = std::time(nullptr) - 86400;
    std::time_t now = std::time(nullptr);
    std::cout << "\n-- Transactions in last 24 hours --\n";
    for (const Transaction& t : ivan->getAccounts()[0]->getTransactionsByDateRange(startOfDay, now))
        std::cout << t << "\n";

    // Monthly statement
    std::cout << "\n";
    ivan->getAccounts()[0]->printMonthlyStatement(5, 2026);
    
    return 0;
}