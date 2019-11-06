#ifndef __LOCK_H__
#define __LOCK_H__

#include <mutex>

class Lock {
    private:
        std::mutex &m;

        Lock(const Lock&) = delete;
        Lock& operator=(const Lock&) = delete;
        Lock(Lock&&) = delete;
        Lock& operator=(Lock&&) = delete;

    public:
        explicit Lock(std::mutex &m);
        ~Lock();
};

#endif
