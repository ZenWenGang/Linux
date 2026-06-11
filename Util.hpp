#pragma once
#include<iostream>
#include<fstream>
#include<string>

//工具类

class Util
{
    public:
    static bool ReadFileContent(const std::string &filename,std::string *out)
    {
        std::ifstream in(filename);
        if(!in.is_open())
        {
            return false;
        }
        std::string line;
        while(std::getline(in,line))
        {
            *out+=line;
        }
        in.close();
        return true;
    }
    static bool ReadOneLine(std::string &bingstr,std::string *out,const std::string &sep)
    {
        auto pos=bingstr.find(sep);
        if(pos==std::string::npos)
        return false;

        *out=bingstr.substr(0,pos);
        bingstr.erase(0,pos+sep.size());
        return true;
    }

};