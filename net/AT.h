#ifndef __AT_H__
#define __AT_H__

#include "Define.h"
#include "serial.h"
#include <iostream>
#include <vector>
#include <queue>

class AT
{
public:
	typedef enum
	{
		AT_GET = 0,
		AT_SET,
		AT_TEST,
	}AType;
private:
    bool logOpen;
	Serial uart;
	AType cmdType;
	std::string cmd;
	std::string param;
    std::string line;
    std::queue<std::string> lineQueue;
    inline void setDataRemaining(const std::string& str){line = str;}
public:
	AT(const char* name, int flags, bool isLog = false):uart(name, flags),logOpen(isLog){}
	AT(const char* name, bool isLog = false):uart(name),logOpen(isLog){}
	~AT(){}
	static std::vector<std::string> split(const std::string& text, char sep = ',', int count = 0);
    int readLines();
	int getline(std::string& oline);
    inline const std::string& getDataRemaining() {return line;}
    inline int getDataRAW(std::string& data){return uart.read(data);};
	int cmdParse(const std::string& str);
	inline int response(const std::string& str){return response(str, cmd);}
	inline int response(const std::string& str, const std::string& cmd){return responseRAW("+" + cmd + ":" + str + "\r\n");}
	int responseRAW(const std::string& str);
    inline int requestRAW(const std::string& str){return responseRAW(str);}
	inline const std::string& Cmd(){return cmd;}
	inline const std::string& Param(){return param;}
	inline AType Type(){return cmdType;} 
	inline int getFd(){return uart.getFd();}
    inline int setToSerial(int nSpeed = 115200, int nBits = 8, char nEvent = 'N', int nStop = 1){ return uart.set_opt(nSpeed, nBits, nEvent, nStop);}
};

#endif

