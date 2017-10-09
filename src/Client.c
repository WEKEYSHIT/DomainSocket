#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "CommonDef.h"
#include "Client.h"

static MesgCb onMessage = NULL;
static volatile int sockfd = -1;
static pthread_t tid = -1;

static void ClientClose()
{
    if(sockfd >= 0)
    {
        close(sockfd);
        sockfd = -1;
        if(onMessage)
            onMessage(NULL, -1);
        onMessage = NULL;
    }
}

static int ClientOpen(const char* domain, int isFile)
{
    ClientClose();
    sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(sockfd < 0)
        return -1;

    printf("domain:%s isFile:%d\n", domain, isFile);

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    address.sun_path[0] = 0;
    strncpy(address.sun_path+!isFile, domain, UNIX_PATH_MAX-1);
    int address_len = isFile ? sizeof(address) : sizeof(address.sun_family) + strlen(domain) + 1;
    int result = connect(sockfd, (struct sockaddr *)&address, address_len);
    if(result < 0)
    {
        ClientClose();
        return -1;
    }
    return sockfd;
}

static void* ClientRun(void* arg)
{
    unsigned char buff[1024];
    while(onMessage)
    {
        int len = read(sockfd, buff, sizeof(buff));
        if(len < 0)
            continue;
        if(len == 0)
            break;
        onMessage(buff, len);
    }
    ClientClose();
}

int ClientStart(MesgCb callback, const char* domain, int isFile)
{
    ClientOpen(domain, isFile);
    if(sockfd >= 0 && callback)
    {
        onMessage = callback;
        int err = pthread_create(&tid, NULL, ClientRun, NULL);
        if(err < 0)
            ClientClose();
    }
    return sockfd;
}


int ClientStop()
{
    ClientClose();
    pthread_join(tid, NULL);
}

