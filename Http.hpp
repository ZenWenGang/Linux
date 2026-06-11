#pragma once

#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Util.hpp"
#include "Log.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <unordered_map>

using namespace SocketModule;
using namespace LogModule;

const std::string gspace = " ";
const std::string glinespace = "\r\n";
const std::string glinesep = ": ";

const std::string webroot = "./wwwroot";
const std::string homepage = "index.html";

class HttpRequest
{
public:
    HttpRequest()
    {
    }
    std::string Serialize()
    {
        return std::string();
    }
    void ParseReqLine(std::string &reqline)
    {
        // GET / HTTP/1.1
        std::stringstream ss(reqline);
        ss >> _method >> _uri >> _version;
    }
    // 实现, 我们今天认为，reqstr是一个完整的http request string
    bool Deserialize(std::string &reqstr)
    {
        // 1. 提取请求行
        std::string reqline;
        bool res = Util::ReadOneLine(reqstr, &reqline, glinespace);
        LOG(LogLevel::DEBUG) << reqline;

        // 2. 对请求行进行反序列化
        ParseReqLine(reqline);

        LOG(LogLevel::DEBUG) << "_method: " << _method;
        LOG(LogLevel::DEBUG) << "_uri: " << _uri;
        LOG(LogLevel::DEBUG) << "_version: " << _version;
        if(_uri == "/")
            _uri = webroot + _uri + homepage; // ./wwwroot/index.html
        else
            _uri = webroot + _uri;// ./wwwroot/a/b/c.html
        return true;
    }
    std::string Uri(){return _uri;}
    ~HttpRequest()
    {
    }

private:
    std::string _method;
    std::string _uri;
    std::string _version;

    std::unordered_map<std::string, std::string> _headers;
    std::string _blankline;
    std::string _text;
};

class HttpResponse
{
public:
    HttpResponse() : _version("HTTP/1.1"), _code(200), _desc("OK"), _blankline(glinespace)
    {
    }
    // 实现: 成熟的http，应答做序列化，不要依赖任何第三方库！
    std::string Serialize()
    {
        std::string status_line = _version + gspace + std::to_string(_code) + gspace + _desc + glinespace;
        std::string resp_header;
        for (auto &header : _headers)
        {
            std::string line = header.first + glinesep + header.second + glinespace;
            resp_header += line;
        }

        return status_line + resp_header + _blankline + _text;
    }
    void SetTargetFile(const std::string &target)
    {
        _targetfile = target;
    }
    bool MakeResponse()
    {
        bool res = Util::ReadFileContent(_targetfile, &_text); // 浏览器请求的资源，一定会存在吗？出错呢？
        if(!res)
        {
            _code = 404;
            _desc = "Not Found";
            _text = "<html><body><h1>404 Not Found</h1></body></html>";

        }else
        {
             _code = 200;
        _desc = "OK";
        }
        _headers["Content-Length"] = std::to_string(_text.size());
        _headers["Content-Type"] = "text/html; charset=utf-8";
        _headers["Connection"] = "close";
        return true;
    }
    bool Deserialize(std::string &reqstr)
    {
        return true;
    }
    ~HttpResponse() {}

    // private:
public:
    std::string _version;
    int _code;         // 404
    std::string _desc; // "Not Found"

    std::unordered_map<std::string, std::string> _headers;
    std::string _blankline;
    std::string _text;

    // 其他属性
    std::string _targetfile;
};

class Http
{
public:
    Http(uint16_t port) : tsvrp(std::make_unique<TcpServer>(port))
    {
    }
    void HandlerHttpRquest(std::shared_ptr<Socket> &sock, InetAddr &client)
    {
        // 收到请求
        std::string httpreqstr;
        // 假设：概率大，读到了完整的请求
        // bug!
        int n = sock->Recv(&httpreqstr); // 浏览器给我发过来的是一个大的http字符串, 其实我们的recv也是有问题的。tcp是面向字节流的.
        if(n > 0)
        {
            // 对报文完整性进行审核 -- 缺
            HttpRequest req;
            req.Deserialize(httpreqstr);
            HttpResponse resp;
            resp.SetTargetFile(req.Uri());
            resp.MakeResponse();

            // HttpResponse resp;
            // resp._version = "HTTP/1.1";
            // resp._code = 200; // success
            // resp._desc = "OK";
            // //./wwwroot/a/b/c.html
            // LOG(LogLevel::DEBUG) << "用户请求: " << filename;
            // bool res = Util::ReadFileContent(filename, &(resp._text)); // 浏览器请求的资源，一定会存在吗？出错呢？
            // (void)res;
            std::string response_str = resp.Serialize();
            sock->Send(response_str);
        }



// #ifndef DEBUG
// #define DEBUG
#ifdef DEBUG
        // 收到请求
        std::string httpreqstr;
        // 假设：概率大，读到了完整的请求
        sock->Recv(&httpreqstr); // 浏览器给我发过来的是一个大的http字符串, 其实我们的recv也是有问题的。tcp是面向字节流的.
        std::cout << httpreqstr;

        // 直接构建http应答. 内存级别+固定
        HttpResponse resp;
        resp._version = "HTTP/1.1";
        resp._code = 200; // success
        resp._desc = "OK";

        std::string filename = webroot + homepage; // "./wwwroot/index.html";
        bool res = Util::ReadFileContent(filename, &(resp._text));
        (void)res;
        std::string response_str = resp.Serialize();
        sock->Send(response_str);
#endif
        // 对请求字符串，进行反序列化
    }
    void Start()
    {
        tsvrp->Start([this](std::shared_ptr<Socket> &sock, InetAddr &client)
                     { this->HandlerHttpRquest(sock, client); });
    }
    ~Http()
    {
    }

private:
    std::unique_ptr<TcpServer> tsvrp;
};
