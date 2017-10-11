#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Domain.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//参数1:收到的二维码解码数据
//参数2:二维码数据的长度
typedef void (*MesgCb)(void*, unsigned char*, int);
int ClientFd(void*);
void* ClientStart(MesgCb callback, void* userdata, const char* domain, int isFile);
void ClientStop(void*);

/*  与扫码服务建立通信通道，返回通道的文件描述符。通道一次传输一包完整数据。
 *  参数:onMessage 回调函数。
 *          NULL:仅建立通信通道。应用程序需要自己去处理读写。
 *          其它:建立通道，并开辟线程去读取消息，并在读取消息后通过回调函数通知应用程序。
*   返回: <0 : 错误
          >=0: 文件描述符
**/
static inline void* ClientStartSimple(MesgCb callback)
{
    return ClientStart(callback, NULL, DEF_DOMAIN, DEF_TYPE);
}


#ifdef __cplusplus
}
#endif

#endif

