#include "Server.h"
#include <string.h>

using namespace std;

void* NewServer(const char* domain, int isFile)
{
    return (void*)new Server(string(domain), isFile);
}

void Broadcast(void* obj, const char* data, int dataLen)
{
    Server* s = static_cast<Server*>(obj);
    s->Broadcast(string(data, dataLen));
}

void DelServer(void* obj)
{
    delete (Server*)obj;
}

