#include "Server.h"
#include <string.h>
#include <stdio.h>

int main()
{
    void* obj1 = NewServer("Qrcode1", 0);
    void* obj2 = NewServer("Qrcode2", 0);
    void* obj3 = NewServerSimple();
    while(1)
    {
        char c = getchar();
        if(c == 'q')
            break;
        printf("--%02x\n",(unsigned char)c);
        char buff[] = "123456\0\0";
        int len = strlen(buff);
        buff[len++] = c;
        Broadcast(obj1, buff, len+1);
        Broadcast(obj2, buff, len+1);
        Broadcast(obj3, buff, len+1);
    }
    DelServer(obj1);
    DelServer(obj2);
    DelServer(obj3);
}

