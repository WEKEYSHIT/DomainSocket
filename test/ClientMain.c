#include "Client.h"
#include <stdio.h>

int err = 0;
void onMessage(void* userdata, unsigned char* message, int mesgLen)
{
    printf("---%s:%s len %d\n",__FUNCTION__, (char*)userdata, mesgLen);
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
    ClientP client1, client2, client3;
    while(NULL == (client1 = ClientOpen(onMessage, "1", "Qrcode1", 0)));
    while(NULL == (client2 = ClientOpen(onMessage, "2", "Qrcode2", 0)));
    while(NULL == (client3 = ClientOpenSimple(onMessage)));
    getchar();
    ClientExit(client1);
    ClientExit(client2);
    ClientExit(client3);
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
