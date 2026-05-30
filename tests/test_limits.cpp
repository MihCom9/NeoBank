#include "TestRunner.h"
#include "account/LimitSettings.h"
#include "account/CheckingAccount.h"
#include "notification/Notification.h"

void runLimitTests(TestRunner& t) {
    t.suite("LimitSettings — checks");

    t.run("Default limits allow any amount", [](){
        LimitSettings ls;
        ASSERT(ls.checkLimit(999999.0));
    });

    t.run("Amount within both limits is allowed", [](){
        LimitSettings ls(500.0, 200.0, 0.0);
        ASSERT(ls.checkLimit(150.0));
    });

    t.run("Single tx limit blocks oversized transaction", [](){
        LimitSettings ls(1000.0, 200.0, 0.0);
        ASSERT(!ls.checkLimit(300.0));
    });

    t.run("Daily limit blocks after accumulation", [](){
        LimitSettings ls(300.0, 200.0, 0.0);
        ls.addSpending(250.0);
        ASSERT(!ls.checkLimit(100.0));
    });

    t.run("Exactly at daily limit is allowed (boundary)", [](){
        LimitSettings ls(300.0, 300.0, 0.0);
        ls.addSpending(200.0);
        ASSERT(ls.checkLimit(100.0)); // 200+100 == 300 == limit, not strictly greater
    });

    t.run("Exceeding daily limit by 1 is blocked", [](){
        LimitSettings ls(300.0, 300.0, 0.0);
        ls.addSpending(200.0);
        ASSERT(!ls.checkLimit(101.0)); // 200+101 = 301 > 300
    });

    t.run("Daily spent accumulates correctly", [](){
        LimitSettings ls(1000.0, 500.0, 0.0);
        ls.addSpending(100.0);
        ls.addSpending(200.0);
        ASSERT_NEAR(ls.getDailySpent(), 300.0, 0.01);
    });

    t.suite("Account — limit notifications");

    t.run("Withdraw over single tx limit creates LIMIT_REACHED notification", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.setLimitSettings(LimitSettings(1000.0, 100.0, 0.0));
        try { acc.withdraw(200.0); } catch (...) {}
        bool found = false;
        for (const auto& n : acc.getNotifications())
            if (n.getType() == NotificationType::LIMIT_REACHED) found = true;
        ASSERT(found);
    });

    t.run("Withdraw over daily limit creates LIMIT_REACHED notification", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.setLimitSettings(LimitSettings(200.0, 300.0, 0.0));
        try { acc.withdraw(100.0); } catch (...) {}
        try { acc.withdraw(150.0); } catch (...) {}
        bool found = false;
        for (const auto& n : acc.getNotifications())
            if (n.getType() == NotificationType::LIMIT_REACHED) found = true;
        ASSERT(found);
    });

    t.run("Balance below min alert creates LOW_BALANCE notification", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.setLimitSettings(LimitSettings(2000.0, 2000.0, 500.0));
        acc.withdraw(600.0); // balance drops to 400 < min 500
        bool found = false;
        for (const auto& n : acc.getNotifications())
            if (n.getType() == NotificationType::LOW_BALANCE) found = true;
        ASSERT(found);
    });

    t.run("Withdraw within limits creates no notification", [](){
        CheckingAccount acc("BG80TEST0000000000001", "BGN", 1000.0, 5000.0, false);
        acc.setLimitSettings(LimitSettings(500.0, 300.0, 0.0));
        acc.withdraw(100.0);
        ASSERT_EQ((int)acc.getNotifications().size(), 0);
    });
}
