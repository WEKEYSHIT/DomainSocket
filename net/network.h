#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <stdlib.h>
#include <string.h>
#include "glib.h"

class Network
{
private:
    bool isRun;
    int runTime;
    int connectTime;
public:
    inline Network():isRun(false){Disconnect();}
    inline ~Network(){Disconnect();}
    inline void connect(){
        if(isRun)   Disconnect();
        int ret = system("dsi_netctrl_dial auto >/dev/null 2>&1 &");
        runTime = lib_timestamp();
        isRun = true;
        connectTime = 0;
    }
    inline bool isConnected()
    {
        if(!isRun) return false;
        FILE* pp = popen("ifconfig rmnet_data0 | grep 'inet addr'","r");
        char buff[256] = {0};
        int ret = fread(buff, sizeof(buff), 1, pp);
        pclose(pp);
        if(connectTime == 0 && buff[0])
            connectTime = lib_timestamp();
        return buff[0];
    }
    inline void Disconnect()
    {
        int ret = system("kill -9 `pgrep dsi_netctrl_dial` >/dev/null 2>&1");
        isRun = false;
    }
    inline u32 getRunTime()
    {
        u32 ret = isRun ? (lib_timestamp() - runTime)/1000 : 0;
        return ret;
    }
    inline u32 getConnectTime()
    {
        u32 ret = isRun && connectTime ? (lib_timestamp() - connectTime)/1000 : 0;
        return ret;
    }
};

#endif

