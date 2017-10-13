#ifndef __CLINET_PLUSPLUS_H__
#define __CLINET_PLUSPLUS_H__

#include "Client.h"

class Client
{
private:
    ClientP clientObj;
    void* data;

    template<class T, void (T::*method)(Client*, unsigned char*, int)>
    static inline void onMessage(void* object, unsigned char* mesg, int mesgLen)
    {
        Client* obj = (Client*)object;
        (static_cast<T*>(obj->data)->*method)(obj, mesg, mesgLen);
    }

    inline bool _rset(MesgCb callback, void* userdata, const char* domain, int isFile)
    {
        ClientExit(clientObj);
        clientObj = ClientOpen(callback, userdata, domain, isFile);
        return clientObj != NULL;
    }

    inline bool _set(const void* userdata, MesgCb cbk, const char* domain, bool isFile)
    {
        data = const_cast<void*>(userdata);
        return _rset(cbk, this, domain, isFile);
    }

public:
    inline Client():clientObj(nullptr),data(nullptr){}
    inline ~Client(){ClientExit(clientObj);}

    template<class T, void (T::*method)(Client*, unsigned char*, int)>
    inline bool Set(const T* object, const char* domain = DEF_DOMAIN, bool isFile = DEF_TYPE)
    {
        return _set(object, onMessage<T, method>, domain, isFile);
    }

    template<void (*method)(Client*, unsigned char*, int)>
    inline bool Set(void* userdata = nullptr, const char* domain =DEF_DOMAIN , bool isFile = DEF_TYPE)
    {
        return _set(userdata, method, domain, isFile);
    }

    template<void (*method)(void*, unsigned char*, int)>
    inline bool Set(void* userdata = nullptr, const char* domain =DEF_DOMAIN , bool isFile = DEF_TYPE)
    {
        return _rset(method, userdata, domain, isFile);
    }
    inline void* userdata(){return this->data;}
};

#endif

