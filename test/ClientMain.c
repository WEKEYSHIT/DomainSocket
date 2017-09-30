#include "Client.h"
#include <stdio.h>

void onMessage(unsigned char* message, int mesgLen)
{
    printf("---%s: len %d\n",__FUNCTION__, mesgLen);
    fwrite(message, mesgLen, 1, stdout);
    fwrite("\n", 1, 1, stdout);
    fflush(stdout);
}

void loop()
{
    while(1) sleep(1);
}
#if 1
int main()
{
    ClientStart(onMessage);
    loop();
}
#else
int main()
{
    int fd = ClientStart(NULL);
    while(1)
    {
        char buff[1024];
        int len = read(fd, buff, sizeof(buff));
        if(len < 0)
        {
            close(fd);
            break;
        }
        onMessage(buff, len);
    }
}
#endif
