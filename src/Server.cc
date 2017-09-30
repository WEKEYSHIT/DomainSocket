#include "Server.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include <iostream>

using namespace std;

Server::Server(const string& domain)
{
    domainPath = domain;
    LoopStart();
}

Server::~Server()
{
    LoopStop();
}

int Server::Open(const string& path)
{
    const char* domain = path.c_str();
    struct sockaddr_un server_address;
    unlink(domain);
    int server_sockfd = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);
    cout << "path: " << domain << "fd: " << server_sockfd << endl;
    server_address.sun_family = AF_UNIX;
    strcpy (server_address.sun_path, domain);
    int server_len = sizeof (server_address);
    cout << "bind:" << ::bind (server_sockfd, (struct sockaddr *)&server_address, server_len)<<endl;
    cout << "listen:" << ::listen (server_sockfd, 5) << endl;
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
        cout << "new FD:" << client_sockfd << " errno: " << errno<<"-" <<strerror(errno) << endl;
        if(client_sockfd < 0)
            continue;
        slist.push_back(client_sockfd);
    }
}

void Server::LoopStart()
{
    sockFd = Open(domainPath);
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
    cout << "send1 " << data << endl;
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
    cout << "send to " << socket << " " << data << endl;
    int len = ::write(socket, data.data(), data.size());
    if(len < 0)
        ::close(socket);
    return len;
}

