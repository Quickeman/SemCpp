#ifndef SEMAPHORE_CPP_H
#define SEMAPHORE_CPP_H

#include <chrono>

/** Semaphore class. */
class Semaphore {
public:
    /** Type used for post count. */
    using count_t = unsigned;

    /** Clock type used for the @ref wait_for and @ref wait_until methods. */
    using clock_t = std::chrono::steady_clock;

    /** Constructor. */
    Semaphore(count_t init = 0);

    /** Destructor. Default. */
    ~Semaphore() = default;

    /** Posts semaphore availability. */
    count_t post(count_t n = 1) noexcept;

    /** @returns the availability count of the semaphore. */
    count_t count() const noexcept;

    /** Checks for semaphore availability.
     * Returns immediately with `true` if available, `false` otherwise. */
    bool check() const noexcept;

    /** Waits for the semaphore to become available. */
    void wait() noexcept;

    /** Waits for the semaphore to become available for @a time.
     * If the semaphore becomes available before @a time has elapsed, returns
     * `true`, otherwise returns `false`. */
    bool wait_for(std::chrono::seconds time);

    /** Waits for the semaphore to become available until @a time.
     * If the semaphore becomes available before @a time, returns `true`,
     * otherwise returns `false`. */
    bool wait_until(std::chrono::time_point<clock_t> time);

private:
    /** Semaphore availability counter. */
    count_t _count;
};

#endif
