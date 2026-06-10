#include "Command.hpp"
#include "TcpServer.hpp"
// #include "Dict.hpp"

std::string defaulthandler(const std::string &word, InetAddr &addr)
{
    LOG(LogLevel::DEBUG) << "回调到了defaulthandler";
    std::string s = "haha, ";
    s += word;
    return s;
}

void Usage(std::string proc)
{
    std::cerr << "Usage: " << proc << " port" << std::endl;
}

// 远程命令执行的功能！
// ./tcpserver port
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = std::stoi(argv[1]);

    Enable_Console_Log_Strategy();

    // // 1. 翻译模块
    // Dict d;
    // d.LoadDict();
    // 1. 命令的执行模块
    Command cmd;
    //  std::string Execute(const std::string &cmd, InetAddr &addr)

    std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port,
                                                                  std::bind(&Command::Execute, &cmd, std::placeholders::_1, std::placeholders::_2));
    // std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port, [&cmd](const std::string &command, InetAddr &addr)
    //                                                               { return cmd.Execute(command, addr); });

    // std::unique_ptr<TcpServer> tsvr = std::make_unique<TcpServer>(port, [&d](const std::string &word, InetAddr &addr){
    //     return d.Translate(word, addr);
    // });
    tsvr->Init();
    tsvr->Run();

    return 0;
}