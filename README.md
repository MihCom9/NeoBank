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
```

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

- Database integration
- Authentication system
- Graphical user interface
- REST API
- Unit testing framework
- Persistent transaction storage

---

## Author

Developed as an Object-Oriented Programming project.