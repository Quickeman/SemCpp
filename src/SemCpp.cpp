#include <SemCpp.hpp>

#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdexcept>
#include <iostream>
#include <thread> // for yield

Semaphore::Semaphore(const std::string& name, Count init):
_name{name} {
    this->_sem = sem_open(this->_name.c_str(), O_RDWR | O_CREAT, S_IRWXU | S_IRGRP, init);

    if (this->_sem == SEM_FAILED) {
        std::string msg {"Failed to open Semaphore " + this->_name + ": "};

        switch (errno) {
            case EINVAL:
            case ENAMETOOLONG:
            case ENOENT:
                if (init > SEM_VALUE_MAX)
                    msg += "initial value too big";
                else
                    msg += "invalid name";
                throw std::invalid_argument(msg);
            break;

            case EACCES:
                msg += "permission denied";
            break;
            case EMFILE:
            case ENFILE:
                msg += "too many files open";
            break;
            case ENOMEM:
                msg += "insufficient memory";
            break;
        }

        throw std::runtime_error(msg);
    }
}

Semaphore::~Semaphore() {
    if (sem_close(this->_sem) == -1) {
        std::cerr << "Semaphore " << this->_name << ": internal error (destructor)\n";
        return;
    }
    if (sem_unlink(this->_name.c_str()) == -1) {
        switch (errno){
        case EACCES:
            std::cerr << "Semaphore " << this->_name << ": permission denied to unlink\n";
        break;
        case ENAMETOOLONG:
            std::cerr << "Semaphore " << this->_name << ": invalid name\n";
        break;
        case ENOENT:
            // Probably already unlinked by another destructor
        break;
        }
    }
}

void Semaphore::post() {
    if (sem_post(this->_sem) == -1) {
        switch (errno) {
            case EINVAL:
                throw std::runtime_error("Semaphore " + this->_name + ": internal error (post)");
            break;
            case EOVERFLOW:
                throw std::overflow_error("Semaphore " + this->_name);
            break;
        }
    }
}

bool Semaphore::check() {
    if (sem_trywait(this->_sem) == -1) {
        if (errno == EINVAL)
            throw std::runtime_error("Semaphore " + this->_name + ": internal error (wait_for/until)");

        return false;
    }
    else
        return true;
}

void Semaphore::wait() {
    if (sem_wait(this->_sem) == -1) {
        std::string msg {"Semaphore " + this->_name + ": "};

        switch (errno) {
            case EINVAL:
                msg += "internal error (wait)";
            break;
            case EINTR:
                msg += "interrupted by signal";
            break;
        }

        throw std::runtime_error(msg);
    }
}

bool Semaphore::wait_until(std::chrono::time_point<Clock> t) {
#ifndef __APPLE__
    // MacOS does not support sem_timedwait, see below
    using namespace std::chrono;

    const auto secs {time_point_cast<seconds>(t)};
    const auto ns {time_point_cast<nanoseconds>(t) - time_point_cast<nanoseconds>(secs)};
    const timespec tmspc {secs.time_since_epoch().count(), ns.count()};

    if (sem_timedwait(this->_sem, &tmspc) == -1) {
        switch (errno) {
            case ETIMEDOUT:
            case EINTR:
                return false;
            break;
            case EINVAL:
                throw std::runtime_error("Semaphore " + this->_name + ": internal error (wait_for/until)");
            break;
        }
    }

    return true;
#else
    // MacOS does not support sem_timedwait
    do {
        if (sem_trywait(this->_sem) == -1) {
            switch (errno) {
                case EAGAIN:
                    // Semaphore unavailable, do nothing; continue with loop
                break;
                case EINTR:
                    return false;
                break;
                case EINVAL:
                    throw std::runtime_error("Semaphore " + this->_name + ": internal error (wait_for/until)");
            }
        }
        else
            return true;

        std::this_thread::yield();
    } while (Clock::now() < t);

    return false;
#endif
}
