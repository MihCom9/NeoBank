#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <ctime>
#include "person/ClientManager.h"
#include "account/CheckingAccount.h"
#include "account/SavingsAccount.h"
#include "account/CreditAccount.h"

// ─── Terminal helpers ──────────────────────────────────────────────────────

static void cls() { std::cout << "\033[2J\033[H"; }

static void pause() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void hdr(const std::string& title) {
    std::cout << "\n\033[1;36m" << std::string(54, '=') << "\n"
              << "  " << title << "\n"
              << std::string(54, '=') << "\033[0m\n";
}

static void sep() { std::cout << "  " << std::string(50, '-') << "\n"; }

static std::string askLine(const std::string& prompt) {
    std::cout << "  " << prompt;
    std::string s;
    std::getline(std::cin >> std::ws, s);
    return s;
}

static int askInt(const std::string& prompt) {
    int v = 0;
    while (true) {
        std::cout << "  " << prompt;
        if (std::cin >> v) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  \033[33m! Enter a whole number.\033[0m\n";
    }
}

static double askDouble(const std::string& prompt) {
    double v = 0.0;
    while (true) {
        std::cout << "  " << prompt;
        if (std::cin >> v) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return v;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  \033[33m! Enter a number.\033[0m\n";
    }
}

// ─── Formatting helpers ────────────────────────────────────────────────────

static std::string accTypeName(const Account* a) {
    if (dynamic_cast<const CheckingAccount*>(a)) return "Checking";
    if (dynamic_cast<const SavingsAccount*>(a))  return "Savings ";
    if (dynamic_cast<const CreditAccount*>(a))   return "Credit  ";
    return "Unknown ";
}

static std::string statusColor(AccountStatus s) {
    switch (s) {
        case AccountStatus::ACTIVE: return "\033[32mACTIVE\033[0m";
        case AccountStatus::LOCKED: return "\033[33mLOCKED\033[0m";
        case AccountStatus::CLOSED: return "\033[31mCLOSED\033[0m";
        default: return "?";
    }
}

static std::string loanStatusColor(LoanStatus s) {
    switch (s) {
        case LoanStatus::ACTIVE:  return "\033[32mACTIVE\033[0m";
        case LoanStatus::PAID:    return "\033[34mPAID\033[0m";
        case LoanStatus::OVERDUE: return "\033[31mOVERDUE\033[0m";
        default: return "?";
    }
}

static std::string money(double v, const std::string& cur = "BGN") {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << v << " " << cur;
    return ss.str();
}

// Collect all accounts across every client in the manager
static std::vector<Account*> collectAllAccounts(ClientManager& mgr) {
    std::vector<Account*> result;
    for (Person* p : mgr.getAll())
        for (Account* a : p->getAccounts())
            result.push_back(a);
    return result;
}

// ─── Account menu ──────────────────────────────────────────────────────────

static void accountMenu(Account* acc, ClientManager& mgr) {
    while (true) {
        cls();
        hdr("Account  " + acc->getIban());
        std::cout << "  Type    : " << accTypeName(acc) << "\n"
                  << "  Balance : \033[1m" << money(acc->getBalance(), acc->getCurrency()) << "\033[0m\n"
                  << "  Status  : " << statusColor(acc->getStatus()) << "\n";

        if (auto* ca = dynamic_cast<CreditAccount*>(acc)) {
            std::cout << "  Credit  : used " << money(ca->getUsedCredit())
                      << "  /  available " << money(ca->getAvailableCredit()) << "\n";
        }
        sep();
        std::cout << "  [1] Deposit          [2] Withdraw\n"
                  << "  [3] Transfer         [4] Transaction history\n"
                  << "  [5] Monthly statement[6] Category report (30d)\n"
                  << "  [7] Set limits       [8] Apply interest / fee\n"
                  << "  [9] Lock             [10] Unlock\n"
                  << "  [11] Close\n";
        sep();

        int ch = askInt("Choice [0=Back]: ");
        if (ch == 0) return;

        try {
            switch (ch) {

            case 1: {
                double amt = askDouble("Amount: ");
                std::string d = askLine("Description (optional): ");
                acc->deposit(amt, d);
                std::cout << "  \033[32m✓ Deposited. Balance: " << money(acc->getBalance(), acc->getCurrency()) << "\033[0m\n";
                break;
            }

            case 2: {
                double amt = askDouble("Amount: ");
                std::string d = askLine("Description (optional): ");
                acc->withdraw(amt, d);
                std::cout << "  \033[32m✓ Withdrawn. Balance: " << money(acc->getBalance(), acc->getCurrency()) << "\033[0m\n";
                break;
            }

            case 3: {
                auto all = collectAllAccounts(mgr);
                std::cout << "\n  Available target accounts:\n";
                bool anyOther = false;
                for (size_t i = 0; i < all.size(); ++i) {
                    if (all[i] == acc) continue;
                    std::cout << "  [" << i << "] "
                              << all[i]->getIban() << " [" << accTypeName(all[i]) << "]  "
                              << money(all[i]->getBalance(), all[i]->getCurrency())
                              << "  " << statusColor(all[i]->getStatus()) << "\n";
                    anyOther = true;
                }
                if (!anyOther) { std::cout << "  No other accounts exist.\n"; break; }
                int idx = askInt("Target index: ");
                if (idx < 0 || idx >= (int)all.size() || all[idx] == acc) {
                    std::cout << "  \033[33m! Invalid selection.\033[0m\n"; break;
                }
                double amt = askDouble("Amount: ");
                std::string d = askLine("Description (optional): ");
                acc->transfer(*all[idx], amt, d);
                std::cout << "  \033[32m✓ Transferred. Your balance: "
                          << money(acc->getBalance(), acc->getCurrency()) << "\033[0m\n";
                break;
            }

            case 4: {
                const auto& txs = acc->getTransactions();
                if (txs.empty()) { std::cout << "  No transactions yet.\n"; break; }
                std::cout << "\n  [1] All  [2] By type  [3] Min amount  [0] Cancel\n";
                int f = askInt("Filter: ");
                if (f == 1) {
                    for (const auto& tx : txs) std::cout << tx << "\n";
                } else if (f == 2) {
                    std::cout << "  Type: [1] DEPOSIT  [2] WITHDRAW  [3] TRANSFER\n";
                    int tp = askInt("Type: ");
                    TransactionType tt = (tp == 1) ? TransactionType::DEPOSIT
                                       : (tp == 2) ? TransactionType::WITHDRAW
                                                   : TransactionType::TRANSFER;
                    for (const auto& tx : acc->getTransactionsByType(tt)) std::cout << tx << "\n";
                } else if (f == 3) {
                    double min = askDouble("Minimum amount: ");
                    for (const auto& tx : acc->getTransactionsByMinAmount(min)) std::cout << tx << "\n";
                }
                break;
            }

            case 5: {
                int m = askInt("Month (1-12): ");
                int y = askInt("Year (e.g. 2026): ");
                acc->printMonthlyStatement(m, y);
                break;
            }

            case 6: {
                std::time_t now  = std::time(nullptr);
                std::time_t from = now - 30 * 24 * 3600;
                std::cout << "  (Last 30 days)\n";
                acc->printCategoryReport(from, now);
                break;
            }

            case 7: {
                double dl = askDouble("Daily spending limit: ");
                double sl = askDouble("Single transaction limit: ");
                double mb = askDouble("Min balance alert: ");
                acc->setLimitSettings(LimitSettings(dl, sl, mb));
                std::cout << "  \033[32m✓ Limits saved.\033[0m\n";
                break;
            }

            case 8:
                acc->applyInterestOrFee();
                std::cout << "  \033[32m✓ Applied. Balance: "
                          << money(acc->getBalance(), acc->getCurrency()) << "\033[0m\n";
                break;

            case 9:
                acc->lock();
                std::cout << "  \033[33m⚠ Account locked.\033[0m\n";
                break;

            case 10:
                acc->unlock();
                std::cout << "  \033[32m✓ Account unlocked.\033[0m\n";
                break;

            case 11:
                acc->close();
                std::cout << "  \033[31m⚠ Account closed.\033[0m\n";
                break;

            default:
                std::cout << "  \033[33m! Unknown option.\033[0m\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  \033[31m✗ " << e.what() << "\033[0m\n";
        }
        pause();
    }
}

// ─── Loan menu ─────────────────────────────────────────────────────────────

static void loanMenu(Person* p) {
    while (true) {
        cls();
        hdr("Loans of " + p->getName());
        auto& loans = p->getLoans();
        if (loans.empty()) {
            std::cout << "  No loans.\n";
            pause();
            return;
        }
        for (size_t i = 0; i < loans.size(); ++i) {
            std::cout << "  [" << i << "]  Remaining: "
                      << std::fixed << std::setprecision(2) << loans[i].getRemainingDebt()
                      << " BGN  |  " << loanStatusColor(loans[i].getStatus()) << "\n";
        }
        sep();
        int idx = askInt("Select loan [−1=Back]: ");
        if (idx < 0) return;
        if (idx >= (int)loans.size()) { std::cout << "  \033[33m! Invalid.\033[0m\n"; pause(); continue; }

        Loan& loan = loans[idx];
        cls();
        hdr("Loan details");
        double mp = loan.calculateMonthlyPayment();
        std::cout << "  Remaining debt    : " << money(loan.getRemainingDebt()) << "\n"
                  << "  Monthly payment   : " << money(mp) << "\n"
                  << "  Status            : " << loanStatusColor(loan.getStatus()) << "\n";
        sep();
        std::cout << "  [1] Make payment  [2] View full schedule  [0] Back\n";
        int ch = askInt("Choice: ");
        try {
            if (ch == 1) {
                double amt = askDouble("Amount (monthly = " + std::to_string(mp) + " BGN): ");
                loan.makePayment(amt);
                std::cout << "  \033[32m✓ Paid. Remaining: " << money(loan.getRemainingDebt()) << "\033[0m\n";
            } else if (ch == 2) {
                auto sched = loan.getPaymentSchedule();
                std::cout << "\n  Payment schedule (" << sched.size() << " installments):\n";
                for (size_t i = 0; i < sched.size(); ++i)
                    std::cout << "    Month " << std::setw(3) << (i + 1) << ":  " << money(sched[i]) << "\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  \033[31m✗ " << e.what() << "\033[0m\n";
        }
        pause();
    }
}

// ─── Open account flow ─────────────────────────────────────────────────────

static void openAccountFlow(Person* p) {
    hdr("Open New Account");
    std::cout << "  [1] Checking  [2] Savings  [3] Credit\n";
    int type = askInt("Account type: ");
    std::string iban = askLine("IBAN (e.g. BG80BNBG96611020000001): ");
    std::string cur  = askLine("Currency (e.g. BGN): ");
    double bal       = askDouble("Initial balance: ");
    try {
        if (type == 1) {
            double dl = askDouble("Daily limit: ");
            bool ovd  = (askInt("Overdraft allowed? 1=yes / 0=no: ") == 1);
            CheckingAccount acc(iban, cur, bal, dl, ovd);
            p->openAccount(&acc);
        } else if (type == 2) {
            double rate = askDouble("Annual interest rate (%): ");
            int    per  = askInt("Compounding periods per year: ");
            SavingsAccount acc(iban, cur, bal, rate, per);
            p->openAccount(&acc);
        } else if (type == 3) {
            double lim  = askDouble("Credit limit: ");
            double pen  = askDouble("Penalty rate (%): ");
            CreditAccount acc(iban, cur, bal, lim, pen);
            p->openAccount(&acc);
        } else {
            std::cout << "  \033[33m! Unknown type.\033[0m\n"; return;
        }
        std::cout << "  \033[32m✓ Account opened.\033[0m\n";
    } catch (const std::exception& e) {
        std::cout << "  \033[31m✗ " << e.what() << "\033[0m\n";
    }
}

// ─── Client menu ───────────────────────────────────────────────────────────

static void clientMenu(Person* p, ClientManager& mgr) {
    while (true) {
        cls();
        hdr("Client: " + p->getName() + "  [" + p->getClientType() + "]");
        std::cout << "  Email    : " << p->getEmail() << "\n"
                  << "  Phone    : " << p->getPhone() << "\n"
                  << "  Accounts : " << p->getAccounts().size() << "\n"
                  << "  Loans    : " << p->getLoans().size() << "\n"
                  << "  Notifs   : " << p->getNotifications().size() << "\n";
        sep();
        std::cout << "  [1] Full profile     [2] Open account\n"
                  << "  [3] Select account   [4] Apply for loan\n"
                  << "  [5] Manage loans     [6] Notifications\n";
        sep();

        int ch = askInt("Choice [0=Back]: ");
        if (ch == 0) return;

        try {
            switch (ch) {

            case 1:
                p->printInfo();
                break;

            case 2:
                openAccountFlow(p);
                break;

            case 3: {
                const auto& accs = p->getAccounts();
                if (accs.empty()) { std::cout << "  No accounts yet.\n"; break; }
                std::cout << "\n";
                for (size_t i = 0; i < accs.size(); ++i)
                    std::cout << "  [" << i << "]  " << accs[i]->getIban()
                              << "  [" << accTypeName(accs[i]) << "]  "
                              << money(accs[i]->getBalance(), accs[i]->getCurrency())
                              << "  " << statusColor(accs[i]->getStatus()) << "\n";
                int idx = askInt("Select account: ");
                if (idx >= 0 && idx < (int)accs.size())
                    accountMenu(accs[idx], mgr);
                else
                    std::cout << "  \033[33m! Invalid selection.\033[0m\n";
                break;
            }

            case 4: {
                double principal = askDouble("Principal amount (BGN): ");
                double rate      = askDouble("Annual interest rate (%): ");
                int    months    = askInt("Term (months): ");
                p->applyForLoan(principal, rate, months);
                double mp = p->getLoans().back().calculateMonthlyPayment();
                std::cout << "  \033[32m✓ Loan approved. Monthly payment: "
                          << money(mp) << "\033[0m\n";
                break;
            }

            case 5:
                loanMenu(p);
                break;

            case 6: {
                const auto& ns = p->getNotifications();
                if (ns.empty()) { std::cout << "  No notifications.\n"; break; }
                for (const auto& n : ns)
                    std::cout << "  [" << n.typeToString() << "]  " << n.getMessage() << "\n";
                break;
            }

            default:
                std::cout << "  \033[33m! Unknown option.\033[0m\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  \033[31m✗ " << e.what() << "\033[0m\n";
        }
        pause();
    }
}

// ─── Main menu ─────────────────────────────────────────────────────────────

static void mainMenu(ClientManager& mgr) {
    while (true) {
        cls();
        hdr("NeoBank Pro");
        const auto& all = mgr.getAll();
        std::cout << "  Registered clients: " << all.size() << "\n";
        sep();
        std::cout << "  [1] Register individual client\n"
                  << "  [2] Register corporate client\n"
                  << "  [3] List all clients\n"
                  << "  [4] Select client\n"
                  << "  [0] Exit\n";
        sep();

        int ch = askInt("Choice: ");

        try {
            switch (ch) {

            case 0:
                std::cout << "\n  Goodbye.\n\n";
                return;

            case 1: {
                hdr("Register Individual Client");
                std::string name  = askLine("Full name: ");
                std::string email = askLine("Email: ");
                std::string phone = askLine("Phone: ");
                std::string egn   = askLine("EGN (10 digits): ");
                std::string dob   = askLine("Date of birth (YYYY-MM-DD): ");
                std::string addr  = askLine("Address: ");
                auto* c = mgr.registerIndividual(name, email, phone, egn, dob, addr);
                std::cout << "  \033[32m✓ Registered: " << c->getName()
                          << "  (ID " << c->getId() << ")\033[0m\n";
                break;
            }

            case 2: {
                hdr("Register Corporate Client");
                std::string name  = askLine("Contact name: ");
                std::string email = askLine("Email: ");
                std::string phone = askLine("Phone: ");
                std::string eik   = askLine("EIK (9 or 13 digits): ");
                std::string comp  = askLine("Company name: ");
                std::string vat   = askLine("VAT number (leave blank if not registered): ");
                std::string rep   = askLine("Legal representative: ");
                auto* c = mgr.registerCorporate(name, email, phone, eik, comp, vat, rep);
                std::cout << "  \033[32m✓ Registered: " << c->getCompanyName()
                          << "  (ID " << c->getId() << ")\033[0m\n";
                break;
            }

            case 3:
                std::cout << "\n";
                mgr.listAll();
                break;

            case 4: {
                if (all.empty()) { std::cout << "  No clients registered yet.\n"; break; }
                std::cout << "\n";
                for (size_t i = 0; i < all.size(); ++i)
                    std::cout << "  [" << i << "]  " << all[i]->getName()
                              << "  (" << all[i]->getClientType() << ")\n";
                int idx = askInt("Select client: ");
                if (idx >= 0 && idx < (int)all.size())
                    clientMenu(all[idx], mgr);
                else
                    std::cout << "  \033[33m! Invalid selection.\033[0m\n";
                break;
            }

            default:
                std::cout << "  \033[33m! Unknown option.\033[0m\n";
            }
        } catch (const std::exception& e) {
            std::cout << "  \033[31m✗ Error: " << e.what() << "\033[0m\n";
        }
        pause();
    }
}

// ─── Demo seed data ────────────────────────────────────────────────────────

static void seed(ClientManager& mgr) {
    auto* ivan = mgr.registerIndividual(
        "Ivan Petrov", "ivan@example.com", "+359888123456",
        "9001011234", "1990-01-01", "Sofia, Bulgaria");
    { CheckingAccount a("BG80BNBG96611020345678", "BGN", 1500.0, 3000.0, false); ivan->openAccount(&a); }
    { SavingsAccount  a("BG80BNBG96611020345679", "BGN", 5000.0, 3.5, 12);       ivan->openAccount(&a); }
    ivan->applyForLoan(10000.0, 5.0, 24);

    auto* firma = mgr.registerCorporate(
        "Georgi Ivanov", "office@techbg.com", "+35929123456",
        "123456789", "TechBG EOOD", "BG123456789", "Georgi Ivanov");
    { CheckingAccount a("BG80BNBG96611020345680", "BGN",  8000.0, 5000.0, true); firma->openAccount(&a); }
    { CreditAccount   a("BG80BNBG96611020345681", "BGN",     0.0, 20000.0, 8.0); firma->openAccount(&a); }
}

// ─── Entry point ───────────────────────────────────────────────────────────

int main() {
    ClientManager mgr;
    seed(mgr);
    mainMenu(mgr);
    return 0;
}
