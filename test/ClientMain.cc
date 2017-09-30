#include "Client++.h"
#include <stdio.h>
#include <unistd.h>

static void onMessage(unsigned char* message, int mesgLen)
{
    printf("---%s: len %d\n",__FUNCTION__, mesgLen);
    fwrite(message, mesgLen, 1, stdout);
    fwrite("\n", 1, 1, stdout);
    fflush(stdout);
}

class Test
{
public:
    Test()
    {
        #if 1
        Client::Start<Test, &Test::onMessage>(this);
        #else
        Client::start(::onMessage);
        #endif
    }
    void loop(){while(1) sleep(1);}

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
