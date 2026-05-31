#include "TestRunner.h"
#include "transaction/Transaction.h"

static Transaction make(const std::string& desc) {
    return Transaction("T1", TransactionType::WITHDRAW, 10.0, "BGN",
                       "BG80TEST0000000000001", "", TransactionStatus::SUCCESSFUL, desc);
}

void runTransactionTests(TestRunner& t) {
    t.suite("Transaction — categorization");

    t.run("Lidl / Kaufland / grocery → FOOD", [](){
        ASSERT(make("Kaufland grocery run").getCategory() == Category::FOOD);
        ASSERT(make("Lidl supermarket").getCategory() == Category::FOOD);
        ASSERT(make("Restaurant dinner").getCategory() == Category::FOOD);
    });

    t.run("Uber / bus / train → TRANSPORT", [](){
        ASSERT(make("Uber ride to airport").getCategory() == Category::TRANSPORT);
        ASSERT(make("Bus transport card").getCategory() == Category::TRANSPORT);
        ASSERT(make("Train ticket Sofia").getCategory() == Category::TRANSPORT);
    });

    t.run("Electricity / water / bill → UTILITIES", [](){
        ASSERT(make("Electricity bill payment").getCategory() == Category::UTILITIES);
        ASSERT(make("Water utility invoice").getCategory() == Category::UTILITIES);
        ASSERT(make("Internet subscription bill").getCategory() == Category::UTILITIES);
    });

    t.run("Netflix / concert / cinema → ENTERTAINMENT", [](){
        ASSERT(make("Netflix subscription").getCategory() == Category::ENTERTAINMENT);
        ASSERT(make("Concert ticket entertainment").getCategory() == Category::ENTERTAINMENT);
        ASSERT(make("Cinema movie night").getCategory() == Category::ENTERTAINMENT);
    });

    t.run("Pharmacy / doctor / medicine → HEALTH", [](){
        ASSERT(make("Pharmacy medicine").getCategory() == Category::HEALTH);
        ASSERT(make("Doctor visit fee").getCategory() == Category::HEALTH);
        ASSERT(make("Dental appointment").getCategory() == Category::HEALTH);
    });

    t.run("Amazon / Zara / online purchase → SHOPPING", [](){
        ASSERT(make("Amazon online purchase").getCategory() == Category::SHOPPING);
        ASSERT(make("Zara clothes shop").getCategory() == Category::SHOPPING);
        ASSERT(make("Online store order").getCategory() == Category::SHOPPING);
    });

    t.run("Unknown description → OTHER", [](){
        ASSERT(make("Transfer ref 9912").getCategory() == Category::OTHER);
        ASSERT(make("").getCategory() == Category::OTHER);
    });

    t.suite("Transaction — validation");

    t.run("Empty id throws", [](){
        ASSERT_THROWS(Transaction("", TransactionType::DEPOSIT, 100.0, "BGN",
            "BG80TEST0000000000001", "", TransactionStatus::SUCCESSFUL));
    });

    t.run("Zero amount throws", [](){
        ASSERT_THROWS(Transaction("T1", TransactionType::DEPOSIT, 0.0, "BGN",
            "BG80TEST0000000000001", "", TransactionStatus::SUCCESSFUL));
    });

    t.run("Negative amount throws", [](){
        ASSERT_THROWS(Transaction("T1", TransactionType::DEPOSIT, -50.0, "BGN",
            "BG80TEST0000000000001", "", TransactionStatus::SUCCESSFUL));
    });

    t.run("Empty source IBAN throws", [](){
        ASSERT_THROWS(Transaction("T1", TransactionType::DEPOSIT, 100.0, "BGN",
            "", "", TransactionStatus::SUCCESSFUL));
    });

    t.suite("Transaction — getters");

    t.run("Getters return correct values", [](){
        Transaction tx("TX99", TransactionType::TRANSFER, 250.0, "BGN",
            "BG80TEST0000000000001", "BG80TEST0000000000002",
            TransactionStatus::SUCCESSFUL, "Rent payment");
        ASSERT_EQ(tx.getId(), std::string("TX99"));
        ASSERT_NEAR(tx.getAmount(), 250.0, 0.01);
        ASSERT(tx.getType() == TransactionType::TRANSFER);
        ASSERT(tx.getStatus() == TransactionStatus::SUCCESSFUL);
        ASSERT_EQ(tx.getCurrency(), std::string("BGN"));
    });
}
