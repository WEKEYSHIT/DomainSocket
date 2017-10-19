#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>
#include "Define.h"

class Serial
{
private:
	int fd;
	int open(const char* name, int flags);
    inline void setBlock(bool isBlock);
public:
    Serial(const char* name, int flags = O_RDWR|O_NOCTTY|O_NDELAY|O_NONBLOCK);
    ~Serial();
	int set_opt(int nSpeed, int nBits, char nEvent, int nStop);
	int write(const std::string& str);
    int read(std::string& str);
	int close();
	inline int getFd(){return fd;}
};
#endif

