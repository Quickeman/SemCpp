#include "SemCpp.hpp"

#include "test.hpp"

#include <unistd.h>
#include <iostream>
#include <thread>

int main() {
    Semaphore sem(semTest::name);

    const auto pid {fork()};
    if (pid > 0) {
        // Parent (this)
    }
    else if (pid == 0) {
        // Child (other)
        char* args[] = {const_cast<char*>("SemCpp_test2"), NULL};
        const auto err {execv("./SemCpp_test2", args)};
        std::cerr << "Failed to launch receiver\n";
        exit(1);
    }
    else {
        // Error
        std::cerr << "Failed to fork sender\n";
        exit(1);
    }

    std::cout << "Sender launched\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));

    sem.post();

    std::cout << "Semaphore posted\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));

    if (sem.check())
        exit(1);
}