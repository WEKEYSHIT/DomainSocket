#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <string>
#include <vector>
#include <map>
#include "AT.h"

class Profile
{
    std::map<std::string, std::string> cfg;
private:
    std::map<std::string, std::string> ParaseConfig(const std::string& conf)
    {
        std::map<std::string, std::string> params;
        auto vec = AT::split(conf,';');
        for(auto it = vec.begin(); it != vec.end(); it++)
        {
            auto kv = AT::split(*it, '=', 1);
            if(!kv[0].size())
                continue;
            if(kv.size() == 2)
                params[kv[0]] = kv[1];
            else
                params[kv[0]] = "";
        }
        return params;
    }

    std::map<std::string, std::string> LoadConfig(const std::string& path, const std::string& mcc, const std::string& mnc)
    {
        AT profile(path.c_str(), O_CREAT);
        profile.readLines();
        std::string line = profile.getDataRemaining();
        do
        {
            if(line[0] == '#')
                continue;
            auto p = ParaseConfig(line);
            if((p["mcc"] == mcc || p["mcc"] == "*")&& (p["mnc"] == mnc || p["mnc"] == "*"))
                return p;
        }while(profile.getline(line) >= 0);
        return std::map<std::string, std::string>();
    }

public:
    Profile(const std::string& path, const std::string& mcc, const std::string& mnc)
    {
        cfg = LoadConfig(path, mcc, mnc);
    }

    std::string operator[](const std::string& key)
    {
        auto it = cfg.find(key);
        if(it == cfg.end())
            return "";
        return it->second;
    }
};

#endif

