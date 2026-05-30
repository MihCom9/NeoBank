#include "TestRunner.h"
#include "person/IndividualClient.h"
#include "person/CorporateClient.h"
#include "person/ClientManager.h"
#include "account/CheckingAccount.h"

void runPersonTests(TestRunner& t) {
    // ── IndividualClient ───────────────────────────────────────────────────
    t.suite("IndividualClient");

    t.run("Valid client created successfully", [](){
        IndividualClient c(1, "Ivan Petrov", "ivan@test.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        ASSERT(c.validate());
        ASSERT(c.getClientType() == "IndividualClient");
        ASSERT(c.isActive());
    });

    t.run("EGN not 10 digits throws", [](){
        ASSERT_THROWS(IndividualClient(1, "Test", "t@t.com", "+359888000001",
            "12345", "1990-01-01", "Sofia"));
    });

    t.run("EGN with letters throws", [](){
        ASSERT_THROWS(IndividualClient(1, "Test", "t@t.com", "+359888000001",
            "900101ABCD", "1990-01-01", "Sofia"));
    });

    t.run("Under 18 years old throws", [](){
        ASSERT_THROWS(IndividualClient(1, "Young", "y@y.com", "+359888000001",
            "1601011234", "2016-01-01", "Sofia"));
    });

    t.run("Age calculated correctly", [](){
        IndividualClient c(1, "Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        ASSERT(c.getAge() >= 35);
    });

    t.run("Empty address throws", [](){
        ASSERT_THROWS(IndividualClient(1, "Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", ""));
    });

    t.run("Deactivate sets isActive to false", [](){
        IndividualClient c(1, "Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        c.deactivate();
        ASSERT(!c.isActive());
    });

    t.run("Activate restores active status", [](){
        IndividualClient c(1, "Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        c.deactivate();
        c.activate();
        ASSERT(c.isActive());
    });

    t.run("Double deactivate throws", [](){
        IndividualClient c(1, "Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        c.deactivate();
        ASSERT_THROWS(c.deactivate());
    });

    t.run("Double activate throws", [](){
        IndividualClient c(1, "Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        ASSERT_THROWS(c.activate());
    });

    // ── CorporateClient ────────────────────────────────────────────────────
    t.suite("CorporateClient");

    t.run("Valid corporate client created", [](){
        CorporateClient c(1, "Georgi", "g@corp.com", "+35929000001",
            "123456789", "Corp EOOD", "BG123456789", "Georgi");
        ASSERT(c.validate());
        ASSERT(c.getClientType() == "CorporateClient");
    });

    t.run("EIK not 9 or 13 digits throws", [](){
        ASSERT_THROWS(CorporateClient(1, "G", "g@c.com", "+35929000001",
            "12345", "Corp", "BG123", "G"));
    });

    t.run("VAT registered when vatNumber non-empty", [](){
        CorporateClient c(1, "Georgi", "g@corp.com", "+35929000001",
            "123456789", "Corp EOOD", "BG123456789", "Georgi");
        ASSERT(c.isVatRegistered());
    });

    t.run("Not VAT registered when vatNumber empty", [](){
        CorporateClient c(1, "Georgi", "g@corp.com", "+35929000001",
            "123456789", "Corp EOOD", "", "Georgi");
        ASSERT(!c.isVatRegistered());
    });

    t.run("13-digit EIK accepted", [](){
        CorporateClient c(1, "Georgi", "g@corp.com", "+35929000001",
            "1234567890123", "Corp EOOD", "", "Georgi");
        ASSERT(c.validate());
    });

    // ── ClientManager ──────────────────────────────────────────────────────
    t.suite("ClientManager");

    t.run("Register individual returns valid pointer", [](){
        ClientManager mgr;
        auto* c = mgr.registerIndividual("Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        ASSERT(c != nullptr);
        ASSERT(c->getId() > 0);
    });

    t.run("findById returns correct client", [](){
        ClientManager mgr;
        auto* c = mgr.registerIndividual("Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        ASSERT(mgr.findById(c->getId()) == c);
    });

    t.run("findById returns nullptr for unknown id", [](){
        ClientManager mgr;
        ASSERT(mgr.findById(9999) == nullptr);
    });

    t.run("Duplicate EGN throws", [](){
        ClientManager mgr;
        mgr.registerIndividual("Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        ASSERT_THROWS(mgr.registerIndividual("Ivan2", "i2@i.com", "+359888000002",
            "9001011234", "1990-01-01", "Plovdiv"));
    });

    t.run("Duplicate EIK throws", [](){
        ClientManager mgr;
        mgr.registerCorporate("G", "g@c.com", "+35929000001",
            "123456789", "Corp", "", "G");
        ASSERT_THROWS(mgr.registerCorporate("G2", "g2@c.com", "+35929000002",
            "123456789", "Corp2", "", "G2"));
    });

    t.run("IDs are unique and auto-incremented", [](){
        ClientManager mgr;
        auto* a = mgr.registerIndividual("A", "a@a.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        auto* b = mgr.registerCorporate("B", "b@b.com", "+35929000001",
            "123456789", "Corp", "", "B");
        ASSERT(a->getId() != b->getId());
    });

    // ── Person account management ──────────────────────────────────────────
    t.suite("Person — account management");

    t.run("openAccount adds account to client", [](){
        ClientManager mgr;
        auto* c = mgr.registerIndividual("Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 0.0, 5000.0, false);
        c->openAccount(&acc);
        ASSERT_EQ((int)c->getAccounts().size(), 1);
    });

    t.run("Multiple accounts can be opened", [](){
        ClientManager mgr;
        auto* c = mgr.registerIndividual("Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        CheckingAccount acc1("BG80TEST0000000000001", "BGN", 0.0, 5000.0, false);
        CheckingAccount acc2("BG80TEST0000000000002", "BGN", 0.0, 5000.0, false);
        c->openAccount(&acc1);
        c->openAccount(&acc2);
        ASSERT_EQ((int)c->getAccounts().size(), 2);
    });

    t.run("applyForLoan adds loan", [](){
        ClientManager mgr;
        auto* c = mgr.registerIndividual("Ivan", "i@i.com", "+359888000001",
            "9001011234", "1990-01-01", "Sofia");
        c->applyForLoan(5000.0, 5.0, 12);
        ASSERT_EQ((int)c->getLoans().size(), 1);
    });
}
