#ifndef __COMMONDEF_H__
#define __COMMONDEF_H__

#include <sys/socket.h>
#include <sys/un.h>

#ifndef UNIX_PATH_MAX
    #define UNIX_PATH_MAX sizeof(((struct sockaddr_un*)0)->sun_path)
#endif

#endif

