#include<jsoncpp/json/json.h>
#include<iostream>
#include<jsoncpp/json/value.h>

int main()
{
    Json::Value json;
    json["x"]=1;
    json["oper"]='+';
    json["y"]=2;
    Json::FastWriter w;
    std::string out=w.write(json);
    std::cout<<out;
    return 0;
}