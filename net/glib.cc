#include "glib.h"
#include <stdio.h>
#include <sys/time.h>

u32 lib_timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000+tv.tv_usec/1000;
}

int lib_hexToStr(char *str, const u8 *hex,int len,int type)
{
	if(hex == NULL || str == NULL)
		return -1;
	const char* hexTable[]={"0123456789ABCDEF","0123456789abcdef"};
	const char* tab = hexTable[type==0];
	while(len--)
	{
		u8 ch = *hex++;
		*str++=tab[ch>>4];
		*str++=tab[ch&0xf];
	}
	return (*str = 0);
}

u32 lib_hexstrToInt(const char* str, int len)
{
	unsigned int hex = 0;
	while(len--)
	{
		u8 ch;
		if(str[0] >= '0' && str[0] <= '9')
			ch = str[0] - '0';
		else if(str[0] >= 'a' && str[0] <= 'z')
			ch = str[0] - ('a' - 10);
		else
			ch = str[0]-('A' - 10);
		str++;
		hex <<= 4;
		hex |= ch;
	}
	return hex;
}

void lib_strToHex(u8* hex, const char* str, int len)
{
	while(len)
	{
		*hex++ = lib_hexstrToInt(str, 2);
		len-=2;
		str+=2;
	}
}

void viewNHex(const char* str, const u8* data, int len)
{
	char buff[len*2+1];
	lib_hexToStr(buff, data, len, 1);
	printf("%s:%s\n", str, buff);	
}
