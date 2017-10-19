#ifndef __APN_H__
#define __APN_H__

#include "AT.h"
#include "profile.h"
#include <map>
#include <vector>
#include <string>

class DialInfo:public AT
{
private:
    class APN
    {
    public:
        APN(const std::string& t="", const std::string& a=""):type(t),name(a){}
        std::string type;
        std::string name;
    };

    std::string imsi;
    std::string iccid;
    std::map<char, APN> apn;
    std::string cops;
    std::string csq;
    std::string apnName, username, password;
public:
    DialInfo():AT("/dev/smd8")
    {
        updateInfo();
    }

    void updateInfo()
    {
        checkSIM();
        reqImsi(&imsi);
        reqIccid(&iccid);
        reqCsq(&csq);
        reqCops(&cops);
        reqApn(&apn);
        Profile proile("/data/profile.ini", imsi.substr(0,3), imsi.substr(3,2));
        setApn(proile["apn"]);
        reqApn(&apn);
        setQCPDPP(proile["username"], proile["password"]);
    }

    void checkSIM()
    {
        while(1)
        {
            std::vector<std::string> q;
            if(atReq("AT+CPIN?", &q))
                if(getValue(q[1]) == "READY")
                    return;
            sleep(1);
        }
    }

    void reqIccid(std::string* iccid)
    {
        std::vector<std::string> q;
        if(atReq("AT+ICCID", &q))
            *iccid = getValue(q[1]);
    }

    void reqImsi(std::string* imsi)
    {
        std::vector<std::string> q;
        if(atReq("AT+CIMI", &q))
            *imsi = getValue(q[1]);
    }

    void reqCops(std::string* cops)
    {
        std::vector<std::string> q;
        if(atReq("AT+COPS=3,2", &q))
        {
            std::vector<std::string> q;
            if(atReq("AT+COPS?", &q))
                *cops = getValue(q[1]);
        }
    }

    void reqCsq(std::string* csq)
    {
        std::vector<std::string> q;
        if(atReq("AT+CSQ", &q))
            *csq = getValue(q[1]);
    }

    void reqApn(std::map<char, APN>* apn)
    {
        std::vector<std::string> q;
        apn->clear();
        if(atReq("AT+CGDCONT?", &q))
        {
            for(int i = 0; i < q.size(); i++)
            {
                auto tokens = split(getValue(q[i]));
                if(tokens.size() < 3)
                    continue;
                const std::string& apnType = tokens[1].substr(1, tokens[1].size()-2);
                const std::string& apnName = tokens[2].substr(1, tokens[2].size()-2);
                (*apn)[tokens[0][0]] = APN(apnType, apnName);
            }
        }
    }

    void setApn(const std::string& apn_name)
    {
        if(apn.size() && apn['1'].name == apn_name)
            return;
        std::vector<std::string> q;
        while(!atReq("AT+CGDCONT=1,\"IPV4V6\",\"" + apn_name + "\"", &q));
    }

    void setQCPDPP(const std::string& username, const std::string& password)
    {
        std::vector<std::string> q;
        std::string cmd;
        if(username.empty() || password.empty())
            cmd = "AT$QCPDPP=1,0";
        else
            cmd = "AT$QCPDPP=1,1,\"" + password + "\",\"" + username + '"';
        while(!atReq(cmd, &q));
    }

    std::string getValue(std::string& str)
    {
        size_t found = str.find(':');
        if(found==std::string::npos)
            return str;
        return str.substr(found+2);
    }

    bool atReq(const std::string& req, std::vector<std::string>* q)
    {
//        std::cout << ">>req:" << req << std::endl;
        requestRAW(req+"\r\n");
        while(1)
        {
            int ret = readLines();
            if(ret < 0)
                return false;
            if(!ret)
                continue;
            std::string data;
            while(getline(data) >= 0)
            {
//                std::cout << "<<resp:" << data << std::endl;
                q->push_back(data);
                if(data == "OK")
                    return true;
                if(data == "ERROR")
                    return false;
            }
        }
    }
};


#endif

