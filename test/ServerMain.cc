#include "Server.h"

#define DOMAIN "/tmp/server_socket"

using namespace std;

int main(int argc, char* argv[])
{
    Server server(DOMAIN);
    while(1)
    {
        char c = getchar();
        if(c == 'q')
            break;
        printf("--%02x\n",(unsigned char)c);
        server.Broadcast(string("123456")+c);
    }
    server.LoopStop();
    return 0;
}
