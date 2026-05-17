# NeoBank
Дигитална банкова платформа
# README.md

````md
# NeoBank Pro

NeoBank Pro is a digital banking system developed in C++ using object-oriented programming principles.

The project simulates core banking functionalities such as client management, account operations, transactions, loans, financial analysis, and notifications.

---

## Features

### Client Management
- Register and manage individual and corporate clients
- Edit client profile information
- Validate email and phone data
- Support multiple accounts per client

### Account Management
- Checking accounts
- Savings accounts
- Credit accounts
- Account status management (active, locked, closed)

### Transactions
- Deposit funds
- Withdraw funds
- Transfer between accounts
- Transaction history and filtering
- Daily transaction limits and validation

### Loans
- Loan application system
- Monthly payment calculation
- Loan payment tracking
- Overdue loan detection

### Notifications
- Limit warnings
- Balance alerts
- Transaction notifications

### Financial Analysis
- Transaction categorization
- Expense summaries
- Monthly statements

---

## Technologies

- C++17
- STL (Standard Template Library)
- Object-Oriented Programming
- CMake
- Git & GitHub

---

## Object-Oriented Concepts Used

- Abstraction
- Inheritance
- Polymorphism
- Encapsulation
- Composition
- Association

---

## Project Structure

```text
NeoBank-Pro/
│
├── README.md
├── CMakeLists.txt
├── docs/
├── include/
├── src/
├── tests/
└── app/
````

---

## Main Class Hierarchy

### Person Hierarchy

```text
Person
├── IndividualClient
└── CorporateClient
```

### Account Hierarchy

```text
Account
├── CheckingAccount
├── SavingsAccount
└── CreditAccount
```

---

## Build Instructions

### Using CMake

```bash
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./NeoBank
```

---

## Future Improvements

* Database integration
* Authentication system
* Graphical user interface
* REST API
* Unit testing framework
* Persistent transaction storage

---

## Author

Developed as an Object-Oriented Programming project.

````

---

# docs/requirements.md

```md
# NeoBank Pro — Requirements

## Overview

NeoBank Pro is a digital banking platform designed using object-oriented programming principles.

The system manages clients, accounts, transactions, loans, notifications, and financial reporting.

---

# Functional Requirements

## 1. Client Management

The system must support:

- Registration of individual clients
- Registration of corporate clients
- Unique client identifiers
- Editing client information
- Email validation
- Phone validation
- Multiple accounts per client
- Client deactivation

---

## 2. Account Management

The system must support:

- Checking accounts
- Savings accounts
- Credit accounts
- Unique IBAN generation
- Balance tracking
- Account statuses:
  - Active
  - Locked
  - Closed
- Account creation and closing

---

## 3. Transactions

The system must support:

- Deposits
- Withdrawals
- Internal transfers
- External IBAN transfers
- Transaction validation
- Insufficient balance detection
- Daily transaction limits
- Atomic balance updates

Each transaction must contain:

- ID
- Type
- Amount
- Currency
- Timestamp
- Source IBAN
- Target IBAN
- Category
- Description
- Status

---

## 4. Transaction History

The system must support:

- Transaction history per account
- Filtering by:
  - Date
  - Type
  - Amount
- Monthly statement generation
- Income and expense summaries

---

## 5. Loan System

The system must support:

- Loan applications
- Interest calculation
- Monthly payment calculation
- Loan payment tracking
- Remaining debt tracking
- Overdue loan detection

Loan statuses:

- Active
- Paid
- Overdue

---

## 6. Financial Analysis

The system must support:

- Automatic transaction categorization
- Expense summaries by category
- Financial reports
- Spending analysis

Example categories:

- Food
- Transport
- Utilities
- Entertainment

---

## 7. Notification System

The system must support:

- Daily spending limits
- Single transaction limits
- Minimum balance alerts
- Notification generation
- Transaction blocking on limit violation

---

## 8. Interest and Fees

### Savings Accounts

Must support:

- Interest rate configuration
- Periodic interest accumulation

### Credit Accounts

Must support:

- Penalty interest
- Credit usage tracking
- Credit limits

---

# Non-Functional Requirements

## Performance

- Fast transaction processing
- Efficient account lookup

## Maintainability

- Modular architecture
- Clear class separation
- Reusable components

## Scalability

- Easy addition of new account types
- Easy addition of new client types

## Security

- Validation of all input data
- Prevention of invalid transactions

---

# Design Requirements

The system must follow:

- Encapsulation
- Inheritance
- Polymorphism
- Abstraction
- Composition
- Single Responsibility Principle

---

# Main Entities

## Abstract Classes

- Person
- Account

## Concrete Classes

### Clients
- IndividualClient
- CorporateClient

### Accounts
- CheckingAccount
- SavingsAccount
- CreditAccount

### Supporting Classes
- Transaction
- Loan
- Notification
- LimitSettings

---

# Suggested Folder Structure

```text
include/
src/
docs/
tests/
app/
````

---

```
```
