#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "CommonDef.h"
#include "Client.h"

#define PUBLIC
#define PRIVATE static

typedef struct
{
    int sockfd;
    int joinable;
    pthread_t tid;
    MesgCb onMessage;
    void* userdata;
}Client;

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
        close(this->sockfd);
        this->sockfd = -1;
        if(this->onMessage)
            this->onMessage(&this->userdata, NULL, -1);
        this->onMessage = NULL;
    }
}

PRIVATE void Client_Constructor(Client* this)
{
    this->sockfd = -1;
    this->onMessage = NULL;
    this->joinable = 0;
}

PRIVATE void Client_Destructor(Client* this)
{
    ClientClose(this);
    if(this->joinable)
        pthread_join(this->tid, NULL);
}

PRIVATE void ClientOpen(Client* this, const char* domain, int isFile)
{
    int sockfd = this->sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(sockfd < 0)
        return;

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    address.sun_path[0] = 0;
    strncpy(address.sun_path+!isFile, domain, UNIX_PATH_MAX-1);
    int address_len = isFile ? sizeof(address) : sizeof(address.sun_family) + strlen(domain) + 1;
    int result = connect(sockfd, (struct sockaddr *)&address, address_len);
    if(result < 0)
    {
        perror(domain);
        ClientClose(this);
    }
}

PUBLIC int ClientFd(void* this)
{
    return this?((Client*)this)->sockfd:-1;
}

PRIVATE void* ClientRun(void* arg)
{
    Client* this = arg;
    unsigned char buff[1024];
    while(this->onMessage)
    {
        int len = read(this->sockfd, buff, sizeof(buff));
        if(len < 0)
            continue;
        if(len == 0)
            break;
        this->onMessage(this->userdata, buff, len);
    }
    ClientClose(this);
}

PUBLIC void* ClientStart(MesgCb callback, void* userdata, const char* domain, int isFile)
{
    if(domain == NULL)
    {
        fprintf(stderr,"domain is NULL!\n");
        exit(-1);
    }
    Client* this = new(Client);
    this->userdata = userdata;
    ClientOpen(this, domain, isFile);
    if(this->sockfd >= 0 && callback)
    {
        this->onMessage = callback;
        int err = pthread_create(&this->tid, NULL, ClientRun, this);
        this->joinable = 0 == err;
        if(err < 0)
            ClientClose(this);
    }
    return this;
}


PUBLIC void ClientStop(void* this)
{
    if(this) del(Client, (Client*)this);
}

