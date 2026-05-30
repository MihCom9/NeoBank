#include "TestRunner.h"
#include "loan/Loan.h"

void runLoanTests(TestRunner& t) {
    t.suite("Loan — calculations");

    t.run("Annuity monthly payment is correct (~888 for 10000/12%/12m)", [](){
        Loan loan(10000.0, 12.0, 12);
        ASSERT_NEAR(loan.calculateMonthlyPayment(), 888.49, 1.0);
    });

    t.run("Zero interest: payment = principal / months", [](){
        Loan loan(1200.0, 0.0, 12);
        ASSERT_NEAR(loan.calculateMonthlyPayment(), 100.0, 0.01);
    });

    t.run("Payment schedule has correct number of installments", [](){
        Loan loan(1000.0, 5.0, 24);
        ASSERT_EQ((int)loan.getPaymentSchedule().size(), 24);
    });

    t.run("All schedule installments equal monthly payment", [](){
        Loan loan(5000.0, 6.0, 12);
        double mp = loan.calculateMonthlyPayment();
        for (double p : loan.getPaymentSchedule())
            ASSERT_NEAR(p, mp, 0.01);
    });

    t.suite("Loan — payments");

    t.run("Make payment reduces remaining debt", [](){
        Loan loan(5000.0, 6.0, 12);
        loan.makePayment(500.0);
        ASSERT(loan.getRemainingDebt() < 5000.0);
    });

    t.run("Full payoff sets status PAID", [](){
        Loan loan(500.0, 0.0, 5);
        for (int i = 0; i < 5; ++i) loan.makePayment(100.0);
        ASSERT(loan.getStatus() == LoanStatus::PAID);
        ASSERT_NEAR(loan.getRemainingDebt(), 0.0, 0.01);
    });

    t.run("Overpayment clamps remaining debt to zero", [](){
        Loan loan(100.0, 0.0, 1);
        loan.makePayment(999.0);
        ASSERT_NEAR(loan.getRemainingDebt(), 0.0, 0.01);
        ASSERT(loan.getStatus() == LoanStatus::PAID);
    });

    t.run("Zero payment throws", [](){
        Loan loan(1000.0, 5.0, 12);
        ASSERT_THROWS(loan.makePayment(0.0));
    });

    t.run("Negative payment throws", [](){
        Loan loan(1000.0, 5.0, 12);
        ASSERT_THROWS(loan.makePayment(-100.0));
    });

    t.suite("Loan — validation");

    t.run("Negative principal throws", [](){
        ASSERT_THROWS(Loan(-100.0, 5.0, 12));
    });

    t.run("Zero principal throws", [](){
        ASSERT_THROWS(Loan(0.0, 5.0, 12));
    });

    t.run("Zero term throws", [](){
        ASSERT_THROWS(Loan(1000.0, 5.0, 0));
    });

    t.run("Negative interest rate throws", [](){
        ASSERT_THROWS(Loan(1000.0, -1.0, 12));
    });

    t.suite("Loan — status");

    t.run("New loan status is ACTIVE", [](){
        Loan loan(1000.0, 5.0, 12);
        ASSERT(loan.getStatus() == LoanStatus::ACTIVE);
    });

    t.run("New loan is not overdue", [](){
        Loan loan(1000.0, 5.0, 12);
        ASSERT(!loan.isOverdue());
    });

    t.run("isOverdue calls checkOverdue automatically", [](){
        Loan loan(1000.0, 5.0, 12);
        // Just created — should still be ACTIVE
        ASSERT(loan.getStatus() == LoanStatus::ACTIVE);
        bool result = loan.isOverdue();
        ASSERT(!result);
    });

    t.run("applyOverduePenalty does nothing if not overdue", [](){
        Loan loan(1000.0, 5.0, 12);
        double before = loan.getRemainingDebt();
        loan.applyOverduePenalty();
        ASSERT_NEAR(loan.getRemainingDebt(), before, 0.01);
    });
}
