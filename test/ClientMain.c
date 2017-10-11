#include "Client.h"
#include <stdio.h>

int err = 0;
void onMessage(void* userdata, unsigned char* message, int mesgLen)
{
    printf("---%s: len %d\n",__FUNCTION__, mesgLen);
    if(mesgLen < 0)
    {
        err = 1;
        return;
    }
        
    fwrite(message, mesgLen, 1, stdout);
    fwrite("\n", 1, 1, stdout);
    fflush(stdout);
}

void loop()
{
    while(!err) sleep(1);
}
#if 1
int main()
{
    while(1)
    {
        void* client = ClientStartSimple(onMessage);
        if(ClientFd(client) < 0)
        {
            ClientStop(client);
            sleep(1);
            continue;
        }
        break;
    }
    loop();
}
#else
int main()
{
    int fd = ClientStartSimple(NULL);
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
