#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "Domain.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* ClientP;

//参数1:用户数据
//参数2:收到的二维码解码数据
//参数3:二维码数据的长度
typedef void (*MesgCb)(void*, unsigned char*, int);
// 打开并设置回调
ClientP ClientOpen(MesgCb callback, void* userdata, const char* domain, int isFile);
// 删除回调
void ClientExit(ClientP);

/*  与扫码服务建立通信通道，返回通道的文件描述符。通道一次传输一包完整数据。
 *  参数:onMessage 回调函数。
 *          NULL:仅建立通信通道。应用程序需要自己去处理读写。
 *          其它:建立通道，并在读取消息后通过回调函数通知应用程序。
 *  返回: ClientP
**/
static inline void ClientOpenSimple(MesgCb callback)
{
    while(ClientOpen(callback, NULL, DEF_DOMAIN, DEF_TYPE) == NULL);
}


#ifdef __cplusplus
}
#endif

#endif

