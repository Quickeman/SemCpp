#include <SemCpp.hpp>

// class BinarySemaphore

BinarySemaphore::BinarySemaphore():
_flag{false} {}

void BinarySemaphore::reset() noexcept {
    this->_flag = false;
}

bool BinarySemaphore::post() noexcept {
    const auto p {this->_flag};
    this->_flag = true;
    return p;
}

bool BinarySemaphore::check() const noexcept {
    return this->_flag;
}

void BinarySemaphore::wait() noexcept {
    while(!this->check()) {}

    // Lower flag
    this->_flag = false;
}

bool BinarySemaphore::wait_for(std::chrono::seconds t) {
    return this->wait_until(clock_t::now() + t);
}

bool BinarySemaphore::wait_until(std::chrono::time_point<clock_t> t) {
    do {
        if (this->check()) {
            this->_flag = false;
            return true;
        }
    } while (clock_t::now() < t);

    return false;
}


// class CountingSemaphore

CountingSemaphore::CountingSemaphore():
_count{0} {}

void CountingSemaphore::reset() noexcept {
    this->_count = 0;
}

CountingSemaphore::count_t CountingSemaphore::post(count_t n) noexcept {
    this->_count += n;
}

CountingSemaphore::count_t CountingSemaphore::count() const noexcept {
    return this->_count;
}

bool CountingSemaphore::check() const noexcept {
    return this->_count > 0;
}

void CountingSemaphore::wait() noexcept {
    while(!this->check()) {}

    // Decrement counter when semaphore becomes available
    this->_count--;
}

bool CountingSemaphore::wait_for(std::chrono::seconds t) {
    return this->wait_until(clock_t::now() + t);
}

bool CountingSemaphore::wait_until(std::chrono::time_point<clock_t> t) {
    do {
        if (this->check()) {
            this->_count--;
            return true;
        }
    } while (clock_t::now() < t);

    return false;
}
