#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Log.hpp"

using namespace LogModule;
using func_t = std::function<std::string(const std::string &)>;

const int defaultfd = -1;

class UdpServer
{
public:
    UdpServer(uint16_t port, func_t func)
        : _sockfd(defaultfd),
          _port(port),
          _isrunning(false),
          _func(func) {}
    void Init()
    {
        // 1.创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "socket error!";
            exit(1);
        }
        LOG(LogLevel::INFO) << "socket success,sockfd:" << _sockfd;
        // 2.绑定socket信息，ip和端口
        // 2.1填充sockaddr_in结构体
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        // IP信息和端口信息一定要发送到网络
        // 本地格式->网络序列
        local.sin_port = htons(_port);
        // IP也是如此，1.IP转成4字节 2.4字节转成网络序列->in_addr_t inet_addr(const char *cp);
        local.sin_addr.s_addr = INADDR_ANY;
        // 绑定套接字
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "bind error";
            exit(2);
        }
        LOG(LogLevel::INFO) << "bind success, sockfd : " << _sockfd;
    }
    void Start() // 服务器启动
    {
        char buffer[1024];
        // 客户端的信息
        struct sockaddr_in peer;

        socklen_t len =sizeof(peer);
        while (1)
        {
            char buffer[1024];
            // 接受数据
            int n = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (n > 0)
            {
                buffer[n] = '\0';
                std::cout << buffer << std::endl;
                sendto(_sockfd, buffer, n, 0, (struct sockaddr *)&peer, len);
            }
        }
    }
    ~UdpServer()
    {
    }

private:
    int _sockfd;
    u_int16_t _port;
    bool _isrunning;
    func_t _func; // 服务器的回调函数，用来对数据进行处理
};