#include "AT.h"

using namespace std;

// 读取多行，并返回读取到的行数
// 返回：-1： IO错误
int AT::readLines()
{
	string str = getDataRemaining();
	if(getDataRAW(str) < 0)
		return -1;
    size_t pos = 0;
    size_t found;
    while((found = str.find("\n", pos)) != string::npos)
    {
        int hasR = 0;
        if(found && str[found-1] == '\r') hasR=1;
        lineQueue.push(str.substr(pos, found - pos - hasR));
        pos = found + 1;
    }
    setDataRemaining(str.substr(pos));
	return lineQueue.size();
}

// 读走一行，并返回剩余行数,空则返回-1
// 返回：-1:无数据 >=0:正确
int AT::getline(string& oline)
{
    if(lineQueue.empty())
        return -1;
    oline = lineQueue.front();
    if(logOpen) cout << ">>`" << oline << "'" << endl;
    lineQueue.pop();
    return lineQueue.size();
}

int AT::cmdParse(const string& str)
{
    if(str.find("AT+") != 0) // "ATXXX"  
	{
        cmd = str;
		param = "";
		cmdType = AT_TEST;
		return 0;
	}
	cmdType = AT_SET;
	size_t found = str.find('=', 3);
	if(found == string::npos)
	{
		cmdType = AT_GET;
		param = "";
		if(str[str.size()-1] == '?')
			cmd = str.substr(3, str.size() - 4);
        else
		    cmd = str.substr(3);
		return 0;
	}
    cmd = str.substr(3, found - 3);
    param = str.substr(found+1);
    return 0;
}

int AT::responseRAW(const string& str)
{
    if(logOpen) cout << "<<`" << str << "'" << endl;
	return uart.write(str);
}

#if 0
vector<string> AT::split(const string& text, char sep, int count)
{
	vector<string> tokens;
	size_t start = 0, end = 0;
	int findN = 0;
	while ((end = text.find(sep, start)) != string::npos)
	{
        size_t t_start = start;
        start = end + 1;
        if(end == t_start)
            continue;
		tokens.push_back(text.substr(t_start, end - t_start));
		if(!count)
			continue;
		if(++findN == count)
			break;
  	}
    if(text.size() && start < text.size()-1)
  	    tokens.push_back(text.substr(start));
  	return tokens;
}
#endif

vector<string> AT::split(const string& text, char sep, int count)
{
	vector<string> tokens;
	size_t start = 0, end = 0;
	int findN = 0;
	while ((end = text.find(sep, start)) != string::npos)
	{
		tokens.push_back(text.substr(start, end - start));
        start = end + 1;
		if(!count)
			continue;
		if(++findN == count)
			break;
  	}
  	tokens.push_back(text.substr(start));
  	return tokens;
}
#if 0

int main(int argc, char* argv[])
{
	AT at;
	string line;
	if(at.getline(line) < 0)
		return -1;
	if(at.cmdParse(line) < 0)
		return -2;
	cout << "cmd type:" << at.cmdType << endl;
	cout << "cmd:" << at.cmd << endl;
	cout << "param:" << at.param << endl;
	cout << "line:" << line << endl;
	return 0;
}

#endif
