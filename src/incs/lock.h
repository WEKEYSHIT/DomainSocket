#ifndef __LOCK_H__
#define __LOCK_H__

#include <atomic>
class SpinLock
{
private:
    std::atomic_flag locked;
public:
    inline SpinLock():locked(ATOMIC_FLAG_INIT){}
    inline void lock(){
        while(locked.test_and_set(std::memory_order_acquire));
    }
    inline void unlock() {
        locked.clear(std::memory_order_release);
    }
};

#endif

