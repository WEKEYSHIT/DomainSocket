#include "Client++.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int err = 0;
void onMessage(void* userdata, unsigned char* message, int mesgLen)
{
    printf("%s:%p len %d\n",__FUNCTION__, userdata, mesgLen);
    if(mesgLen < 0)
    {
        err = 1;
        return;
    }

    fwrite(message, mesgLen, 1, stdout);
    fwrite("\n", 1, 1, stdout);
    fflush(stdout);
}

class Test
{
public:
    Test()
    {
        for( ; client1.Fd() < 0; sleep(1))
        {
        #if 1
            client1.Start<Test, &Test::onMessage>(this, "test1");
        #else
            client1.Start<&::onMessage>();
        #endif
        }
        for( ; client2.Fd() < 0; sleep(1))
        {
        #if 1
            client2.Start<Test, &Test::onMessage>(this, "test2");
        #else
            client2.Start<&::onMessage>();
        #endif
        }
        printf("client1:%p client2:%p\n", &client1, &client2);
    }
    void loop(){while(!err) sleep(1);}

private:
    char buff[1024];
    Client client1;
    Client client2;
    void onMessage(Client* c, unsigned char* message, int mesgLen)
    {
        printf("--client:%p\n", c); 
        memcpy(buff, message, mesgLen);
        ::onMessage(c->userdata(), message, mesgLen);
    }

};

int main()
{
    Test test;
    test.loop();
}
