#ifndef __SERVER_H__
#define __SERVER_H__

#ifdef __cplusplus

#include <string>
#include <thread>
#include "SafeList.h"

class Server
{
public:
    Server(const std::string& domain);
    ~Server();
    void Broadcast(const std::string& data);
private:
    bool isRun;
    SafeList slist;
    int sockFd;
    std::string domainPath;
    std::thread thrd;

    void LoopStart();
    void LoopStop();
    int Open(const std::string& path);
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
void* newServer(const char* domain, int len);
void Broadcast(void* obj, char* data, int dataLen);
void delServer(void* obj);
#ifdef __cplusplus
}
#endif

#endif

