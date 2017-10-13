#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "CommonDef.h"
#include "Client.h"
#include "ioManager.h"

#define PUBLIC
#define PRIVATE static
#define STATIC

typedef struct
{
    int sockfd;
    MesgCb onMessage;
    void* userdata;
    int error;
}Client;
PRIVATE pthread_t tid;
PRIVATE int volatile loopRun = 0;

PRIVATE void ClientLoopStart(int fd, void* data);
PRIVATE void ClientLoopExit(int fd);

//#define CONSTRUCTOR(STRUCT, OBJ) STRUCT##_Constructor(OBJ) 
#define CONSTRUCTOR(STRUCT, OBJ)  
#define DESTRUCTOR(STRUCT, OBJ) STRUCT##_Destructor(OBJ)

#define new(STRUCT) ({                                          \
        STRUCT* obj = malloc(sizeof(STRUCT));                   \
        CONSTRUCTOR(STRUCT, obj);                               \
        obj;                                                    \
    })
#define del(STRUCT,OBJ) {DESTRUCTOR(STRUCT,OBJ);free(OBJ);}

PRIVATE void ClientClose(Client* this)
{
    if(this->sockfd >= 0)
    {
        ClientLoopExit(this->sockfd);
        close(this->sockfd);
        this->sockfd = -1;
        if(this->error)
            this->onMessage(&this->userdata, NULL, this->error);
    }
}

PRIVATE void Client_Constructor(Client* this)
{
    this->sockfd = -1;
    this->onMessage = NULL;
    this->error = 0;
}

PRIVATE void Client_Destructor(Client* this)
{
    ClientClose(this);
}

PRIVATE int SocketOpen(const char* domain, int isFile)
{
    int sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(sockfd < 0)
    {
        perror("create socket fail");
        return -1;
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    address.sun_path[0] = 0;
    strncpy(address.sun_path+!isFile, domain, UNIX_PATH_MAX-1);
    int address_len = isFile ? sizeof(address) : sizeof(address.sun_family) + strlen(domain) + 1;
    int result = connect(sockfd, (struct sockaddr *)&address, address_len);
    if(result < 0)
    {
        perror(domain);
        close(sockfd);
        sockfd = -1;
    }
    return sockfd;
}

PRIVATE void ClientOnMessage(void* obj)
{
    Client* this = (Client*)obj;
    unsigned char buff[1024];
    int len = read(this->sockfd, buff, sizeof(buff));
    if(len < 0)
    {
        perror(__FUNCTION__);
        return;
    }
    if(len == 0)
    {
        this->error = -1;
        return ClientClose(this);
    }
    this->onMessage(this->userdata, buff, len);
}

PUBLIC void* ClientOpen(MesgCb callback, void* userdata, const char* domain, int isFile)
{
    if(domain == NULL || callback == NULL)
    {
        fprintf(stderr,"domain(%p) or callback(%p) is NULL!\n", domain, callback);
        exit(-1);
    }
    int fd = SocketOpen(domain, isFile);
    if(fd < 0) return NULL;
    Client* this = new(Client);
    this->sockfd = fd;
    this->userdata = userdata;
    this->onMessage = callback;
    ClientLoopStart(this->sockfd, this);
    return this;
}

PUBLIC void ClientExit(void* this)
{
    if(this) del(Client, (Client*)this);
}

PRIVATE void* ClientLoopRun(void* arg)
{
    loop_event(ClientOnMessage);
}

PRIVATE void ClientLoopStart(int fd, void* data)
{
    if(loopRun++) goto end;
    loop_init();
    if(pthread_create(&tid, NULL, ClientLoopRun, NULL) < 0)
    {
        perror(__FUNCTION__);
        exit(-1);
    }
end:
    add_event(fd, data);
}

PRIVATE void ClientLoopExit(int fd)
{
    if(!loopRun) return;
    del_event(fd);
    loopRun--;
    if(!loopRun)
    {
        loop_exit();
        pthread_join(tid, NULL);
    }
}

