#include "Client++.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int err = 0;
static void onMessage(unsigned char* message, int mesgLen)
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

class Test
{
public:
    Test()
    {
        for( ; Client::Fd() < 0; sleep(1))
        {
        #if 1
            Client::Start<Test, &Test::onMessage>(this);
        #else
            Client::start(::onMessage);
        #endif
        }
    }
    void loop(){while(!err) sleep(1);}

private:
    void onMessage(unsigned char* message, int mesgLen)
    {
        ::onMessage(message, mesgLen);
    }

};

int main()
{
    Test test;
    test.loop();
}
