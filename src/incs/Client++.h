#ifndef __CLINET_PLUSPLUS_H__
#define __CLINET_PLUSPLUS_H__

#include "Client.h"
#include <stdio.h>

class Client
{
private:
    static int fd;
    static void* obj;

    template<class T, void (T::*method)(unsigned char*, int)>
    static inline void onMessage(unsigned char* mesg, int mesgLen)
    {
        (static_cast<T*>(obj)->*method)(mesg, mesgLen);
    }

public:
    template<class T, void (T::*method)(unsigned char*, int)>
    static inline int Start(const T* object)
    {
        obj = (void*)object;
        fd = ClientStart(onMessage<T, method>);
        return fd;
    }

    static inline int Start(MesgCb cbk)
    {
        fd = ClientStart(cbk);
        return fd;
    }

    static inline int Fd(){return fd;}
};
int Client::fd = 0;
void* Client::obj = NULL;

#endif

