#include <sys/epoll.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "ioManager.h"

static int volatile epollfd = -1;
static int pipefd[2];

void loop_init()
{
    if(epollfd < 0)
    {
        epollfd = epoll_create(16);
        int ret= pipe(pipefd);
        struct epoll_event ev = 
        {
            .events = EPOLLIN,
            .data.ptr = pipefd,
        };
        epoll_ctl(epollfd, EPOLL_CTL_ADD, pipefd[0], &ev);
    }
}

static int handle_events(struct epoll_event *events, int num, IO_Callback cbk)
{
    while(num--)
    {
        struct epoll_event *ev = events++;
        
        if(ev->events & EPOLLIN)
        {
            void* data = ev->data.ptr;
            if(data == pipefd)
                return -1;
            cbk(data);
        }
    }
    return 0;
}

void loop_event(IO_Callback cbk)
{
    while(1)
    {
        struct epoll_event events[20];
        int ret = epoll_wait(epollfd, events, sizeof(events)/sizeof(events[0]), -1);
        if(ret < 0)
        {
            perror(__FUNCTION__);
            exit(-1);
        }
        if(handle_events(events, ret, cbk) < 0)
            break;
    }
    close(pipefd[0]);
    close(pipefd[1]);
    close(pipefd[epollfd]);
    epollfd = -1;
}

void loop_exit()
{
    int ret = write(pipefd[1], "", 1);
}

void add_event(int fd, void* data)
{
    struct epoll_event ev = 
    {
        .events = EPOLLIN,
        .data.ptr = data,
    };
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void del_event(int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

