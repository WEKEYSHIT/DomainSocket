#include <iostream>

#include <string.h>

#include "CommonDef.h"
#include "Server.h"

using namespace std;

Server::Server(const string& domain, bool isFile)
{
    domainPath = domain;
    this->isFile = isFile;
    LoopStart();
}

Server::~Server()
{
    LoopStop();
}

int Server::Open(const string& path, bool isFile)
{
    const char* domain = path.c_str();
    int server_sockfd = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);
    struct sockaddr_un server_address;
    server_address.sun_family = AF_UNIX;
    server_address.sun_path[0] = 0;
    strncpy(server_address.sun_path+!isFile, domain, UNIX_PATH_MAX-1);
    cout << "fd:" << server_sockfd << " isFile:" << isFile << " domain:" << domain << endl;
    int address_len = sizeof(server_address.sun_family) + strlen(domain) + 1;
    if(isFile)
    {
        address_len = sizeof(server_address);
        unlink(domain);
    }
    int b=::bind (server_sockfd, (struct sockaddr *)&server_address, address_len);
    int l=::listen (server_sockfd, 5);
    cout << "b:" << b << " l:" << l << endl;
    return server_sockfd;
}

void Server::Loop()
{
    cout << __FUNCTION__ << endl;
    int server_sockfd = sockFd;
    while(isRun)
    {
        struct sockaddr_un client_address;
        int client_len = sizeof(client_address);
        int client_sockfd = ::accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t *)&client_len);
 //       cout << "new FD:" << client_sockfd << " errno: " << errno<<"-" <<strerror(errno) << endl;
        if(client_sockfd < 0)
            continue;
        slist.push_back(client_sockfd);
    }
}

void Server::LoopStart()
{
    sockFd = Open(domainPath, isFile);
    isRun = true;
//    Loop();
    thrd = std::thread(thrdRun<Server, &Server::Loop>, this);
}

void Server::LoopStop()
{
    isRun = false;
    ::close(sockFd);
    sockFd = -1;
    thrd.join();
}

void Server::Broadcast(const string& data)
{
//    cout << "send1 " << data << endl;
    auto it = slist.begin(); 
    while(it != slist.end())
    {
        int len = Send(*it, data);
        if(len < 0)
        {
            it = slist.erase(it);
            continue;
        }
        it++;
    }
}

int Server::Send(int socket, const string& data)
{
//    cout << "send to " << socket << " " << data << endl;
    int len = ::write(socket, data.data(), data.size());
    if(len < 0)
        ::close(socket);
    return len;
}

