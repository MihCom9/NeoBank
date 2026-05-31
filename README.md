# NeoBank Pro

A digital banking platform built in C++17 using object-oriented design principles. Implements 8 core banking functionalities via an interactive terminal interface.

---

## Features

| # | Functionality | Description |
|---|---|---|
| 1 | **Client Management** | Register individual and corporate clients. Validate EGN (10 digits, 18+) and EIK (9 or 13 digits). Edit profile, activate/deactivate accounts. |
| 2 | **Account Management** | Open checking, savings, and credit accounts. Change status: active → locked → active, or close permanently. |
| 3 | **Transactions** | Deposit, withdraw, and transfer between accounts. Validates balance, account status, and daily/single-transaction limits atomically. |
| 4 | **Transaction History** | View all transactions per account. Filter by type, minimum amount, or date range. Generate monthly statements with income/expense totals. |
| 5 | **Loan System** | Apply for loans with configurable principal, rate, and term. Calculates monthly payment via annuity formula. Tracks remaining debt, payments, and overdue status. |
| 6 | **Financial Analysis** | Automatic keyword-based categorization (Food, Transport, Utilities, Entertainment, Health, Shopping). Bar chart expense report per category for any time period. |
| 7 | **Limits & Notifications** | Per-account daily spending limit, single-transaction limit, and minimum balance alert. Notifications recorded on both account and client level. |
| 8 | **Interest & Fees** | Savings accounts accrue periodic interest. Checking accounts deduct a monthly maintenance fee. Credit accounts accumulate penalty interest on used credit. Overdue loans apply penalty on remaining debt. |

---

## Project Structure

```
NeoBank/
├── app/
│   └── main.cpp              # Interactive menu UI
├── include/
│   ├── account/              # Account.h, CheckingAccount.h, SavingsAccount.h,
│   │                         # CreditAccount.h, LimitSettings.h
│   ├── enums/                # AccountStatus, TransactionType, TransactionStatus,
│   │                         # LoanStatus, Category
│   ├── loan/                 # Loan.h
│   ├── notification/         # Notification.h
│   ├── person/               # Person.h, IndividualClient.h, CorporateClient.h,
│   │                         # ClientManager.h
│   └── transaction/          # Transaction.h
├── src/                      # Implementation files (mirrors include/)
├── tests/
│   ├── TestRunner.h          # Lightweight test framework
│   ├── main_test.cpp         # Test entry point
│   ├── test_account.cpp      # Account, CreditAccount, SavingsAccount, CheckingAccount
│   ├── test_person.cpp       # IndividualClient, CorporateClient, ClientManager
│   ├── test_loan.cpp         # Loan calculations, payments, status
│   ├── test_transaction.cpp  # Transaction categorization and validation
│   └── test_limits.cpp       # LimitSettings and notifications
├── docs/
└── CMakeLists.txt
```

---

## Class Hierarchy

```
Person (abstract)
├── IndividualClient   — EGN, date of birth, address, age validation
└── CorporateClient    — EIK, VAT number, representative

Account (abstract)
├── CheckingAccount    — daily limit, overdraft, monthly fee
├── SavingsAccount     — interest rate, compounding period
└── CreditAccount      — credit limit, penalty rate, used credit

Transaction            — type, amount, category, IBAN, timestamp
Loan                   — principal, annuity payment, payment history, overdue detection
LimitSettings          — daily/single-tx limits, auto daily reset
Notification           — type, message, read status
ClientManager          — client registry, duplicate prevention, ID assignment
```

---

## Build

```bash
mkdir build && cd build
cmake ..
make
```

This produces two executables in `build/`:

| Executable | Command | Purpose |
|---|---|---|
| `NeoBank` | `./NeoBank` | Interactive banking application |
| `NeoBank_Tests` | `./NeoBank_Tests` | Component test suite (93 tests) |

---

## Running the App

The app starts with two demo clients pre-loaded:

- **Ivan Petrov** — individual client, checking + savings account, one active loan
- **TechBG EOOD** — corporate client, checking + credit account

Navigate with the numbered menus. All operations include validation and error feedback.

---

## Running the Tests

```bash
./build/NeoBank_Tests
```

Tests are organized in suites and cover normal paths, edge cases, and error handling:

```
[Suite] Account — deposit / withdraw       (8 tests)
[Suite] Account — transfer                 (4 tests)
[Suite] Account — status                   (6 tests)
[Suite] CreditAccount                      (7 tests)
[Suite] SavingsAccount                     (4 tests)
[Suite] CheckingAccount — maintenance fee  (2 tests)
[Suite] IndividualClient                   (10 tests)
[Suite] CorporateClient                    (5 tests)
[Suite] ClientManager                      (6 tests)
[Suite] Person — account management        (3 tests)
[Suite] Loan — calculations                (4 tests)
[Suite] Loan — payments                    (5 tests)
[Suite] Loan — validation                  (4 tests)
[Suite] Loan — status                      (4 tests)
[Suite] Transaction — categorization       (7 tests)
[Suite] Transaction — validation           (4 tests)
[Suite] Transaction — getters              (1 test)
[Suite] LimitSettings — checks             (6 tests)
[Suite] Account — limit notifications      (4 tests)
```

---

## Technologies

- C++17
- STL (vectors, maps, strings, chrono)
- CMake 3.10+
- Object-Oriented Design: abstraction, inheritance, polymorphism, encapsulation, composition

---

## Author

Developed as an Object-Oriented Programming project.
