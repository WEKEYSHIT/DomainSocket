#ifndef __SAFELIST_H__
#define __SAFELIST_H__

#include <list>
#include "lock.h"

//template<typename T>
class SafeList
{
private:
    typedef int T;
    typedef std::list<T> Tlist;
    typedef Tlist::iterator TlistIt;
    typedef Tlist::const_iterator TlistItC;
    Tlist slist;
    SpinLock lock;
public:
    inline void push_back(const T& val)
    {
        lock.lock();
        slist.push_back(val);
        lock.unlock();
    }
    inline TlistIt erase(const TlistIt& pos)
    {
        lock.lock();
        auto ret = slist.erase(pos);
        lock.unlock();
        return ret;
    }
    inline TlistIt begin(){return slist.begin();} 
    inline TlistIt end(){return slist.end();} 
    inline int size()
    {
        lock.lock();
        auto ret = slist.size();
        lock.unlock();
        return ret;
    }
};

#endif

