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
```
# Compiler Requirements

* C++17 or newer
* GCC / Clang / MSVC
* CMake 3.10+