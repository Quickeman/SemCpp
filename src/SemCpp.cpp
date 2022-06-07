#include <SemCpp.hpp>

Semaphore::Semaphore(count_t i):
_count{i} {}

Semaphore::count_t Semaphore::post(count_t n) noexcept {
    this->_count += n;
}

Semaphore::count_t Semaphore::count() const noexcept {
    return this->_count;
}

bool Semaphore::check() const noexcept {
    return this->_count > 0;
}

void Semaphore::wait() noexcept {
    while(!this->check()) {}

    // Decrement counter when semaphore becomes available
    this->_count--;
}

bool Semaphore::wait_for(std::chrono::seconds t) {
    return this->wait_until(clock_t::now() + t);
}

bool Semaphore::wait_until(std::chrono::time_point<clock_t> t) {
    do {
        if (this->check()) {
            this->_count--;
            return true;
        }
    } while (clock_t::now() < t);

    return false;
}
