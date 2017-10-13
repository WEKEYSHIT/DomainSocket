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
        while(!client1.Set<Test, &Test::onMessage>(this, "test1"));
        while(!client2.Set<Test, &Test::onMessage>(this, "test2"));
        while(!client3.Set<&::onMessage>());
        printf("client1:%p client2:%p client3:%p\n", &client1, &client2, &client3);
    }
    void loop(){while(!err) sleep(1);}

private:
    char buff[1024];
    Client client1;
    Client client2;
    Client client3;
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
