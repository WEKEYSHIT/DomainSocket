#include "Server.h"
#include <string.h>
#include <stdio.h>

#define DOMAIN "/tmp/server_socket"

int main()
{
    void* obj = newServer(DOMAIN, strlen(DOMAIN));
    while(1)
    {
        char c = getchar();
        if(c == 'q')
            break;
        printf("--%02x\n",(unsigned char)c);
        char buff[] = "123456\0\0";
        int len = strlen(buff);
        buff[len++] = c;
        Broadcast(obj, buff, len+1);
    }
    delServer(obj);
}
