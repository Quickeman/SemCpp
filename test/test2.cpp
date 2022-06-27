#include "SemCpp.hpp"

#include "test.hpp"

#include <thread>
#include <iostream>

int main() {
    std::cout << "Receiver launched\n";

    Semaphore sem(semTest::name, 0);

    if (sem.check())
        exit(1);

    sem.wait();

    std::cout << "Semaphore acquired by receiver\n";
}