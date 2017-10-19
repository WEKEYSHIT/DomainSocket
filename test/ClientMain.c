#include "Client.h"
#include <stdio.h>

void onMessage(void* userdata, unsigned char* message, int mesgLen)
{
    if(mesgLen < 0)
    {
        printf("error!\n");
        exit(-1);
        return;
    }
        
    printf("len: %d data:\n", mesgLen);
    fwrite(message, mesgLen, 1, stdout);
    fflush(stdout);
    printf("\n");
}


int main(int argc, char* argv[])
{
    ClientOpenSimple(onMessage);
    getchar();
    return 0;
}

