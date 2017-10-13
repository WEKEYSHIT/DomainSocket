#include "Server.h"

using namespace std;

int main(int argc, char* argv[])
{
    Server server1("test1");
    Server server2("test2");
    Server server3;
    while(1)
    {
        char c = getchar();
        if(c == 'q')
            break;
        server1.Broadcast(string("123456")+c);
        server2.Broadcast(string("123456")+c);
        server3.Broadcast(string("123456")+c);
    }
    server1.LoopStop();
    server2.LoopStop();
    server3.LoopStop();
    return 0;
}

