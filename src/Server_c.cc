#include "Server.h"
#include <string.h>

using namespace std;

void* newServer(const char* domain, int len)
{
    return (void*)new Server(string(domain, len));
}

void Broadcast(void* obj, const char* data, int dataLen)
{
    Server* s = static_cast<Server*>(obj);
    s->Broadcast(string(data, dataLen));
}

void delServer(void* obj)
{
    delete (Server*)obj;
}

