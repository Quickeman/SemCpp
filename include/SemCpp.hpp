#ifndef SEMAPHORE_CPP_H
#define SEMAPHORE_CPP_H

#include <semaphore.h>
#include <string>
#include <chrono>

/** Semaphore class. */
class Semaphore {
public:
    /** Type used for semaphore count. */
    using Count = unsigned;

    /** Clock type used for the @ref wait_for and @ref wait_until methods. */
    using Clock = std::chrono::system_clock;

    /** Constructor.
     * Opens the named POSIX semaphore for use. Creates it if it does not exist. */
    Semaphore(const std::string& name, Count init = 0);

    /** Destructor.
     * Removes the POSIX semaphore from the kernel.
     * Other processes may still use the semaphore until all processes using it
     * have exit, but no new references to the semaphore may be established. */
    ~Semaphore();

    /** Posts semaphore availability. */
    void post();

    /** Checks for semaphore availability, consuming availability if there is any.
     * Returns immediately with `true` if available, `false` otherwise. */
    bool check();

    /** Waits for the semaphore to become available. */
    void wait();

    /** Waits for the semaphore to become available for @a time.
     * If the semaphore becomes available before @a time has elapsed, returns
     * `true`, otherwise returns `false`. */
    template<class R, class P>
    bool wait_for(std::chrono::duration<R, P> time);

    /** Waits for the semaphore to become available until @a time.
     * If the semaphore becomes available before @a time, returns `true`,
     * otherwise returns `false`. */
    bool wait_until(std::chrono::time_point<Clock> time);

private:
    /** POSIX semaphore object. */
    sem_t* _sem;

    /** Name of the semaphore. */
    const std::string _name;
};


template<class R, class P>
bool Semaphore::wait_for(std::chrono::duration<R, P> time) {
    return this->wait_until(Clock::now() + time);
}

#endif
