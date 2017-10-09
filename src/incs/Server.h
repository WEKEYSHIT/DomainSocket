#ifndef __SERVER_H__
#define __SERVER_H__

#include "Domain.h"

#ifdef __cplusplus

#include <string>
#include <thread>
#include "SafeList.h"

class Server
{
public:
    Server(const std::string& domain = DEF_DOMAIN, bool isFile = DEF_TYPE);
    ~Server();
    void Broadcast(const std::string& data);
    void LoopStop();
private:
    bool isRun;
    SafeList slist;
    int sockFd;
    std::string domainPath;
    bool isFile;
    std::thread thrd;

    void LoopStart();
    int Open(const std::string& path, bool isFile);
    void Loop();
    int Send(int socket, const std::string& data);

    template<class T, void (T::*method)()>
    static inline void* thrdRun(T* obj)
    {
        (obj->*method)();
    }
};
#endif
#ifdef __cplusplus
extern "C" {
#endif
void* NewServer(const char* domain, int isFile);
void Broadcast(void* obj, const char* data, int dataLen);
void DelServer(void* obj);
inline void* NewServerSimple()
{
    return NewServer(DEF_DOMAIN, DEF_TYPE);
}
#ifdef __cplusplus
}
#endif

#endif

