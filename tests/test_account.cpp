#include "TestRunner.h"
#include "account/CheckingAccount.h"
#include "account/SavingsAccount.h"
#include "account/CreditAccount.h"

void runAccountTests(TestRunner& t) {
    // ── CheckingAccount / base Account ────────────────────────────────────
    t.suite("Account — deposit / withdraw");

    t.run("Deposit increases balance", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.deposit(200.0, "salary");
        ASSERT_NEAR(acc.getBalance(), 1200.0, 0.01);
    });

    t.run("Deposit records transaction", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.deposit(50.0, "bonus");
        ASSERT_EQ((int)acc.getTransactions().size(), 1);
    });

    t.run("Deposit zero throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        ASSERT_THROWS(acc.deposit(0.0));
    });

    t.run("Deposit negative throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        ASSERT_THROWS(acc.deposit(-100.0));
    });

    t.run("Withdraw decreases balance", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.withdraw(300.0);
        ASSERT_NEAR(acc.getBalance(), 700.0, 0.01);
    });

    t.run("Withdraw insufficient funds throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 100.0, 5000.0, false);
        ASSERT_THROWS(acc.withdraw(200.0));
    });

    t.run("Withdraw from locked account throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.lock();
        ASSERT_THROWS(acc.withdraw(100.0));
    });

    t.run("Deposit to closed account throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.close();
        ASSERT_THROWS(acc.deposit(100.0));
    });

    // ── Transfer ───────────────────────────────────────────────────────────
    t.suite("Account — transfer");

    t.run("Transfer moves funds correctly", [](){
        CheckingAccount src("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        CheckingAccount dst("BG80TEST0000000000002", "BGN",  500.0, 5000.0, false);
        src.transfer(dst, 200.0, "rent");
        ASSERT_NEAR(src.getBalance(), 800.0, 0.01);
        ASSERT_NEAR(dst.getBalance(), 700.0, 0.01);
    });

    t.run("Transfer records transaction on both sides", [](){
        CheckingAccount src("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        CheckingAccount dst("BG80TEST0000000000002", "BGN",  500.0, 5000.0, false);
        src.transfer(dst, 100.0);
        ASSERT_EQ((int)src.getTransactions().size(), 1);
        ASSERT_EQ((int)dst.getTransactions().size(), 1);
    });

    t.run("Transfer to locked account throws", [](){
        CheckingAccount src("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        CheckingAccount dst("BG80TEST0000000000002", "BGN",  500.0, 5000.0, false);
        dst.lock();
        ASSERT_THROWS(src.transfer(dst, 200.0));
    });

    t.run("Transfer to closed account throws", [](){
        CheckingAccount src("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        CheckingAccount dst("BG80TEST0000000000002", "BGN",  500.0, 5000.0, false);
        dst.close();
        ASSERT_THROWS(src.transfer(dst, 200.0));
    });

    // ── Status ─────────────────────────────────────────────────────────────
    t.suite("Account — status");

    t.run("Lock sets status LOCKED", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.lock();
        ASSERT(acc.getStatus() == AccountStatus::LOCKED);
    });

    t.run("Close sets status CLOSED", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.close();
        ASSERT(acc.getStatus() == AccountStatus::CLOSED);
    });

    t.run("Close already closed account throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.close();
        ASSERT_THROWS(acc.close());
    });

    t.run("Lock closed account throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.close();
        ASSERT_THROWS(acc.lock());
    });

    t.run("Double lock throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.lock();
        ASSERT_THROWS(acc.lock());
    });

    t.run("Unlock restores ACTIVE status", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.lock();
        acc.unlock();
        ASSERT(acc.getStatus() == AccountStatus::ACTIVE);
    });

    t.run("Unlock allows deposits again", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.lock();
        acc.unlock();
        acc.deposit(100.0);
        ASSERT_NEAR(acc.getBalance(), 1100.0, 0.01);
    });

    t.run("Unlock already active account throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        ASSERT_THROWS(acc.unlock());
    });

    t.run("Unlock closed account throws", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.close();
        ASSERT_THROWS(acc.unlock());
    });

    // ── CreditAccount ──────────────────────────────────────────────────────
    t.suite("CreditAccount");

    t.run("Withdraw from balance only when sufficient", [](){
        CreditAccount acc("BG80TEST0000000000003", "BGN", 500.0, 1000.0, 5.0);
        acc.withdraw(300.0);
        ASSERT_NEAR(acc.getBalance(), 200.0, 0.01);
        ASSERT_NEAR(acc.getUsedCredit(), 0.0, 0.01);
    });

    t.run("Withdraw uses credit when balance insufficient", [](){
        CreditAccount acc("BG80TEST0000000000003", "BGN", 100.0, 1000.0, 5.0);
        acc.withdraw(600.0);
        ASSERT_NEAR(acc.getBalance(), 0.0, 0.01);
        ASSERT_NEAR(acc.getUsedCredit(), 500.0, 0.01);
    });

    t.run("Withdraw over combined limit throws", [](){
        CreditAccount acc("BG80TEST0000000000003", "BGN", 100.0, 500.0, 5.0);
        ASSERT_THROWS(acc.withdraw(700.0));
    });

    t.run("Available credit decreases with use", [](){
        CreditAccount acc("BG80TEST0000000000003", "BGN", 0.0, 1000.0, 5.0);
        acc.withdraw(300.0);
        ASSERT_NEAR(acc.getAvailableCredit(), 700.0, 0.01);
    });

    t.run("Penalty applied on used credit", [](){
        CreditAccount acc("BG80TEST0000000000003", "BGN", 0.0, 1000.0, 10.0);
        acc.withdraw(200.0);
        double before = acc.getUsedCredit();
        acc.applyInterestOrFee();
        ASSERT(acc.getUsedCredit() > before);
    });

    t.run("No penalty when no credit used", [](){
        CreditAccount acc("BG80TEST0000000000003", "BGN", 500.0, 1000.0, 10.0);
        acc.applyInterestOrFee();
        ASSERT_NEAR(acc.getUsedCredit(), 0.0, 0.01);
    });

    t.run("Transfer uses credit when balance insufficient", [](){
        CreditAccount src("BG80TEST0000000000003", "BGN", 100.0, 1000.0, 5.0);
        CheckingAccount dst("BG80TEST0000000000004", "BGN",    0.0, 5000.0, false);
        src.transfer(dst, 600.0);
        ASSERT_NEAR(src.getBalance(), 0.0, 0.01);
        ASSERT_NEAR(src.getUsedCredit(), 500.0, 0.01);
        ASSERT_NEAR(dst.getBalance(), 600.0, 0.01);
    });

    // ── SavingsAccount ─────────────────────────────────────────────────────
    t.suite("SavingsAccount");

    t.run("Interest accrual increases balance", [](){
        SavingsAccount acc("BG80TEST0000000000005", "BGN", 10000.0, 12.0, 12);
        double before = acc.getBalance();
        acc.applyInterestOrFee();
        ASSERT(acc.getBalance() > before);
    });

    t.run("Interest calculation: 12% / 12 periods = 1% per period", [](){
        SavingsAccount acc("BG80TEST0000000000005", "BGN", 10000.0, 12.0, 12);
        acc.applyInterestOrFee();
        ASSERT_NEAR(acc.getBalance(), 10100.0, 0.01);
    });

    t.run("Interest records a transaction", [](){
        SavingsAccount acc("BG80TEST0000000000005", "BGN", 10000.0, 12.0, 12);
        acc.applyInterestOrFee();
        ASSERT_EQ((int)acc.getTransactions().size(), 1);
    });

    t.run("Zero interest rate produces no accrual", [](){
        SavingsAccount acc("BG80TEST0000000000005", "BGN", 10000.0, 0.0, 12);
        double before = acc.getBalance();
        acc.applyInterestOrFee();
        ASSERT_NEAR(acc.getBalance(), before, 0.01);
    });

    // ── CheckingAccount fee ────────────────────────────────────────────────
    t.suite("CheckingAccount — maintenance fee");

    t.run("Monthly fee deducted from balance", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 100.0, 5000.0, false);
        double before = acc.getBalance();
        acc.applyInterestOrFee();
        ASSERT(acc.getBalance() < before);
    });

    t.run("Fee skipped with notification when balance too low", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 0.5, 5000.0, false);
        acc.applyInterestOrFee();
        ASSERT(acc.getNotifications().size() > 0);
    });
}
