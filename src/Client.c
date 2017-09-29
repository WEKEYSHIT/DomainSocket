#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "Client.h"

#define DOMAIN "/tmp/server_socket"

static MesgCb onMessage = NULL;
static volatile int sockfd = -1;

static void ClientClose()
{
    if(sockfd >= 0)
    {
        close(sockfd);
        sockfd = -1;
    }
}

static int ClientOpen(const char* domain, int len)
{
    ClientClose();
    struct sockaddr_un address;
    sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);

    if(sockfd < 0)
    {
        return -1;
    }

    address.sun_family = AF_UNIX;
    strcpy (address.sun_path, domain);

    int result = connect (sockfd, (struct sockaddr *)&address, sizeof (address));
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

int ClientStart(MesgCb callback)
{
    ClientOpen(DOMAIN, strlen(DOMAIN));
    if(sockfd >= 0 && callback)
    {
        pthread_t tid;
        onMessage = callback;
        int err = pthread_create(&tid, NULL, ClientRun, NULL);
        if(err < 0)
            ClientClose();
    }
    return sockfd;
}

