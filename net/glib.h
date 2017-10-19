#ifndef __GLIB_H__
#define __GLIB_H__

#include "Define.h"

int lib_hexToStr(char *str, const u8 *hex,int len,int type);
void lib_strToHex(u8* hex, const char* str, int len);
u32 lib_hexstrToInt(const char* str, int len);
void viewNHex(const char* str, const u8* data, int dataLen);
u32 lib_timestamp(void);

#define BUILD16(A) (((A)[0]<<8)|((A)[1]))
#define BUILD32(A) ((A)[0]<<24)|((A)[1]<<16)|((A)[2]<<8)|((A)[3])

#endif
