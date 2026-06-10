#pragma once
#include "Common.hpp"
#include "Log.hpp"
#include "InetAddr.hpp"
#include "ThreadPool.hpp"
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

// 服务器往往是禁止拷贝的
using namespace LogModule;
using namespace ThreadPoolModule;

// using task_t = std::function<void()>;
using func_t = std::function<std::string(const std::string &, InetAddr &)>;

const static int defaultsockfd = -1;
const static int backlog = 8;

class TcpServer : public NoCopy
{
public:
    TcpServer(uint16_t port, func_t func) : _port(port),
                                            _listensockfd(defaultsockfd),
                                            _isrunning(false),
                                            _func(func)
    {
    }
    void Init()
    {
        // signal(SIGCHLD, SIG_IGN); // 忽略SIG_IGN信号，推荐的做法
        // 1. 创建套接字文件
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (_listensockfd < 0)
        {
            LOG(LogLevel::FATAL) << "socket error";
            exit(SOCKET_ERR);
        }
        LOG(LogLevel::INFO) << "socket success: " << _listensockfd; // 3

        // 2. bind众所周知的端口号
        InetAddr local(_port);
        int n = bind(_listensockfd, local.NetAddrPtr(), local.NetAddrLen());
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "bind error";
            exit(BIND_ERR);
        }
        LOG(LogLevel::INFO) << "bind success: " << _listensockfd; // 3

        // 3. 设置socket状态为listen
        n = listen(_listensockfd, backlog);
        if (n < 0)
        {
            LOG(LogLevel::FATAL) << "listen error";
            exit(LISTEN_ERR);
        }
        LOG(LogLevel::INFO) << "listen success: " << _listensockfd; // 3
    }

    class ThreadData
    {
    public:
        ThreadData(int fd, InetAddr &ar, TcpServer *s) : sockfd(fd), addr(ar), tsvr(s)
        {
        }

    public:
        int sockfd;
        InetAddr addr;
        TcpServer *tsvr;
    };

    // 短服务
    // 长服务: 多进程多线程比较合适
    void Service(int sockfd, InetAddr &peer)
    {
        char buffer[1024];
        while (true)
        {
            // 1. 先读取数据
            // a. n>0: 读取成功
            // b. n<0: 读取失败
            // c. n==0: 对端把链接关闭了，读到了文件的结尾 --- pipe
            ssize_t n = read(sockfd, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                // buffer是一个英文单词 or 是一个命令字符串
                buffer[n] = 0; // 设置为C风格字符串， n<= sizeof(buffer)-1
                LOG(LogLevel::DEBUG) << peer.StringAddr() << " #" << buffer;

                std::string echo_string = _func(buffer, peer);

                // // 2. 写回数据
                // std::string echo_string = "echo# ";
                // echo_string += buffer;

                write(sockfd, echo_string.c_str(), echo_string.size());
            }
            else if (n == 0)
            {
                LOG(LogLevel::DEBUG) << peer.StringAddr() << " 退出了...";
                close(sockfd);
                break;
            }
            else
            {
                LOG(LogLevel::DEBUG) << peer.StringAddr() << " 异常...";
                close(sockfd);
                break;
            }
        }
    }

    static void *Routine(void *args)
    {
        pthread_detach(pthread_self());
        ThreadData *td = static_cast<ThreadData *>(args);
        td->tsvr->Service(td->sockfd, td->addr);
        delete td;
        return nullptr;
    }

    void Run()
    {
        _isrunning = true;
        while (_isrunning)
        {
            // a. 获取链接
            struct sockaddr_in peer;
            socklen_t len = sizeof(sockaddr_in);
            // 如果没有连接，accept就会阻塞
            int sockfd = accept(_listensockfd, CONV(peer), &len);
            if (sockfd < 0)
            {
                LOG(LogLevel::WARNING) << "accept error";
                continue;
            }
            InetAddr addr(peer);
            LOG(LogLevel::INFO) << "accept success, peer addr : " << addr.StringAddr();

            // version2: 多线程版本
            ThreadData *td = new ThreadData(sockfd, addr, this);
            pthread_t tid;
            pthread_create(&tid, nullptr, Routine, td);
        }
        _isrunning = false;
    }
    ~TcpServer()
    {
    }

private:
    uint16_t _port;
    int _listensockfd; // 监听socket
    bool _isrunning;

    func_t _func; // 设置回调处理
};