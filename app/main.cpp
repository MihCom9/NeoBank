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
    ivan->getAccounts()[0]->deposit(500.0, "Salary deposit");
    std::cout << "After deposit: " << ivan->getAccounts()[0]->getBalance() << "\n";

    // Withdraw
    ivan->getAccounts()[0]->withdraw(200.0, "Kaufland grocery shopping");
    std::cout << "After withdraw: " << ivan->getAccounts()[0]->getBalance() << "\n";

    // Transfer from ivan to firma
    ivan->getAccounts()[0]->transfer(*firma->getAccounts()[0], 100.0, "Utility bill payment");
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

    // --- Functionality 6: Financial analysis and categorization ---
    std::cout << "\n-- Functionality 6: Category report --\n";

    // Open a fresh checking account for firma to demo categorization
    CheckingAccount* demoAcc = new CheckingAccount("BG80BNBG96611020345681", "BGN", 3000.0, 1000.0, false);
    firma->openAccount(demoAcc);
    delete demoAcc;
    Account* acc = firma->getAccounts()[1];

    acc->withdraw(45.0,  "Lidl grocery run");
    acc->withdraw(30.0,  "Uber ride to airport");
    acc->withdraw(80.0,  "Electricity bill payment");
    acc->withdraw(25.0,  "Netflix subscription");
    acc->withdraw(60.0,  "Pharmacy medicine");
    acc->withdraw(120.0, "Amazon online purchase");
    acc->withdraw(35.0,  "Restaurant dinner");
    acc->withdraw(20.0,  "Bus transport card");
    acc->withdraw(15.0,  "Concert ticket entertainment");
    acc->withdraw(50.0,  "Zara clothes shop");

    std::time_t from = std::time(nullptr) - 86400;
    std::time_t to   = std::time(nullptr) + 1;
    acc->printCategoryReport(from, to);

    // --- Functionality 7: Limits and notifications ---
    std::cout << "\n-- Functionality 7: Limits and notifications --\n";

    CheckingAccount* limitAcc = new CheckingAccount("BG80BNBG96611020345682", "BGN", 2000.0, 500.0, false);
    ivan->openAccount(limitAcc);
    delete limitAcc;
    Account* lacc = ivan->getAccounts()[2];

    // Set limits: daily 300, single tx 200, min balance alert 1800
    lacc->setLimitSettings(LimitSettings(300.0, 200.0, 1800.0));

    // Withdraw 100 — under limits, balance stays above 1800 — no notification
    lacc->withdraw(100.0, "Kaufland groceries");
    std::cout << "Balance after 100 withdraw: " << lacc->getBalance() << " BGN\n";

    // Withdraw 150 — balance drops to 1750 < 1800 — LOW_BALANCE notification
    lacc->withdraw(150.0, "Zara shopping");
    std::cout << "Balance after 150 withdraw: " << lacc->getBalance() << " BGN\n";

    // Exceeds single tx limit (> 200) — should throw and add LIMIT_REACHED notification
    try {
        lacc->withdraw(250.0, "Big purchase");
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // Exceeds daily limit (100 + 150 = 250 spent; 300 - 250 = 50 left; 80 > 50) — LIMIT_REACHED
    try {
        lacc->withdraw(80.0, "Another shop");
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // Print account notifications
    std::cout << "\nAccount notifications (" << lacc->getNotifications().size() << "):\n";
    for (const Notification& n : lacc->getNotifications())
        std::cout << "  [" << n.typeToString() << "] " << n.getMessage() << "\n";

    // Person-level notifications
    ivan->addNotification(Notification("Welcome to NeoBank!", NotificationType::GENERAL));
    std::cout << "\nPerson notifications (" << ivan->getNotifications().size() << "):\n";
    for (const Notification& n : ivan->getNotifications())
        std::cout << "  [" << n.typeToString() << "] " << n.getMessage() << "\n";

    // --- Functionality 5: Loan system ---
    std::cout << "\n-- Loan system --\n";

    // Apply for a loan: 10 000 BGN, 5% annual interest, 24 months
    firma->applyForLoan(10000.0, 5.0, 24);
    Loan& loan = firma->getLoans()[0];

    std::cout << "Monthly payment: " << loan.calculateMonthlyPayment() << " BGN\n";
    std::cout << "Remaining debt:  " << loan.getRemainingDebt() << " BGN\n";

    // Print first 3 installments from the payment schedule
    std::vector<double> schedule = loan.getPaymentSchedule();
    std::cout << "Payment schedule (" << schedule.size() << " installments):\n";
    for (int i = 0; i < 3; ++i)
        std::cout << "  Month " << (i + 1) << ": " << schedule[i] << " BGN\n";
    std::cout << "  ...\n";

    // Make two payments
    loan.makePayment(loan.calculateMonthlyPayment());
    loan.makePayment(loan.calculateMonthlyPayment());
    std::cout << "After 2 payments, remaining debt: " << loan.getRemainingDebt() << " BGN\n";

    // checkOverdue — loan just started so should still be ACTIVE
    loan.checkOverdue();
    std::cout << "Status: " << (loan.getStatus() == LoanStatus::ACTIVE ? "ACTIVE" : "OVERDUE") << "\n";
    std::cout << "isOverdue: " << (loan.isOverdue() ? "yes" : "no") << "\n";

    // Try invalid loan
    try {
        firma->applyForLoan(-500.0, 5.0, 12);
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // Pay off a second loan fully
    firma->applyForLoan(500.0, 3.0, 6);
    Loan& smallLoan = firma->getLoans()[1];
    double monthly = smallLoan.calculateMonthlyPayment();
    for (int i = 0; i < 6; ++i)
        smallLoan.makePayment(monthly);
    std::cout << "Small loan status: " << (smallLoan.getStatus() == LoanStatus::PAID ? "PAID" : "ACTIVE") << "\n";
    std::cout << "Remaining debt: " << smallLoan.getRemainingDebt() << " BGN\n";

    return 0;
}