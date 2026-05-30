#include <iostream>
#include "TestRunner.h"

void runAccountTests(TestRunner&);
void runPersonTests(TestRunner&);
void runLoanTests(TestRunner&);
void runTransactionTests(TestRunner&);
void runLimitTests(TestRunner&);

int main() {
    std::cout << "\n\033[1mNeoBank Pro — Component Tests\033[0m\n"
              << std::string(44, '=') << "\n";

    TestRunner t;
    runAccountTests(t);
    runPersonTests(t);
    runLoanTests(t);
    runTransactionTests(t);
    runLimitTests(t);
    t.summary();

    return t.exitCode();
}
