#include <sys/epoll.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "ioManager.h"

static void** volatile ioMap = NULL;
static int volatile ioMapSize = 0;
static int volatile epollfd = -1;
static int pipefd[2];
static pthread_mutex_t mutex_x= PTHREAD_MUTEX_INITIALIZER;
#define lock() pthread_mutex_lock(&mutex_x)
#define unlock() pthread_mutex_unlock(&mutex_x)

static void addMap(int fd, void* data)
{
    lock();
    if(ioMapSize <= fd)
    {
        int newSize = fd + 16;
        ioMap = realloc(ioMap, newSize*sizeof(ioMap));
        memset(ioMap + ioMapSize, 0, (newSize-ioMapSize)*sizeof(ioMap));
        ioMapSize = newSize;
    }
    unlock();
    ioMap[fd] = data?data:ioMap;
}

static void delMap(int fd)
{
    lock();
    if(fd < ioMapSize)
        ioMap[fd] = NULL;
    unlock();
}

static int isInMap(int fd)
{
    lock();
    int ret = fd < ioMapSize ? (ioMap[fd] != NULL) : 0;
    unlock();
    return ret;
}

static void* getMapData(int fd)
{
    lock();
    void* ret = ioMap[fd] == ioMap ? NULL : ioMap[fd];
    unlock();
    return ret;
}

void loop_init()
{
    if(epollfd < 0)
    {
        epollfd = epoll_create(16);
        pipe(pipefd);
        struct epoll_event ev = 
        {
            .events = EPOLLIN,
            .data.fd = pipefd[0],
        };
        epoll_ctl(epollfd, EPOLL_CTL_ADD, pipefd[0], &ev);
    }
}

static void handle_events(struct epoll_event *events, int num, IO_Callback cbk)
{
    for(int i = 0; i < num; i++)
    {
        int fd = events[i].data.fd;
        if(!isInMap(fd))
            continue;
        if ((events[i].events & EPOLLIN)&&cbk)
            cbk(getMapData(fd));
    }
}

void loop_event(IO_Callback cbk)
{
    while(epollfd >= 0)
    {
        struct epoll_event events[20];
        int ret = epoll_wait(epollfd, events, sizeof(events)/sizeof(events[0]), -1);
        char buff[100];
        if(ret < 0)
        {
            perror(__FUNCTION__);
            exit(-1);
        }
        handle_events(events, ret, cbk);
    }
}

void loop_exit()
{
    int fd = epollfd;
    epollfd = -1;
    close(fd);
    lock();
    ioMapSize = 0;
    free(ioMap);
    ioMap = NULL;
    unlock();
    write(pipefd[1], "", 1);
}

void add_event(int fd, void* data)
{
    struct epoll_event ev = 
    {
        .events = EPOLLIN,
        .data.fd = fd,
    };
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == 0);
        addMap(fd, data);
}

void del_event(int fd)
{
    struct epoll_event ev = 
    {
        .events = EPOLLIN,
        .data.fd = fd,
    };
    if(epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev) == 0)
        delMap(fd);
}

