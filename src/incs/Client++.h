#ifndef __CLINET_PLUSPLUS_H__
#define __CLINET_PLUSPLUS_H__

#include "Client.h"

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
    static inline int Start(const T* object, const char* domain = DEF_DOMAIN, bool isFile = DEF_TYPE)
    {
        obj = (void*)object;
        fd = ClientStart(onMessage<T, method>, domain, isFile);
        return fd;
    }

    static inline int Start(MesgCb cbk, const char* domain =DEF_DOMAIN , bool isFile = DEF_TYPE)
    {
        fd = ClientStart(cbk, domain, isFile);
        return fd;
    }

    static inline int Fd(){return fd;}
};
int Client::fd = -1;
void* Client::obj = nullptr;

#endif

