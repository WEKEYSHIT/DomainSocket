#include "DialInfo.h"
#include "network.h"
#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
    DialInfo info;
    Network net;
    net.connect();
    Server s("netstat");
    while(1)
    {
        if(net.isConnected())
        {
            s.Broadcast("connected");
            sleep(5);
            continue;
        }
        s.Broadcast("disconnected");
        net.connect();
        sleep(10);
    }
    return 0;
}

