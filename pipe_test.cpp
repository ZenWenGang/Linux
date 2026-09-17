#include<iostream>
#include<unistd.h>
#include<sys/wait.h>
#include<cstring>
#include<cstdio>
using namespace std;

int main()
{
    //创建管道
    int pipe_fd[2];
    if(pipe(pipe_fd)==-1)
    {
        cerr<<"管道创建失败"<<endl;
        return -1;
    }
    //创建子进程
    pid_t pid=fork();
    if(pid==-1)
    {
        cerr<<"子进程创建失败"<<endl;
        return -1;
    }

    if(pid==0)//子进程
    {
        //只读，关闭写
        close(pipe_fd[1]);
        //接收数据的缓冲区
        char buffer[1024]={0};
        //从管道读取数据
        ssize_t len=read(pipe_fd[0],buffer,size(buffer));
        //判断是否读取成功
        if(len>0)
        {
            cout<<"[子进程]收到父进程消息:"<<buffer<<endl;
            cout<<"[子进程PID:]"<<getpid()<<endl;
        }
        //关闭读端
        close(pipe_fd[0]);
        return 0;
    }
    else//父进程
    {
        //只写，关闭读
        close(pipe_fd[0]);
        //要发送的数据
        const char* msg="你好！我是父进程，这是管道通信测试~";
        //向管道写入数据
        write(pipe_fd[1],msg,strlen(msg));
        cout<<"[父进程]已经发送消息:"<<msg<<endl;
        cout<<"[父进程]PID:"<<getpid()<<endl;
        //关闭写
        close(pipe_fd[1]);
        //等待子进程，防止僵尸进程
        waitpid(pid,nullptr,0);
    }
    cout<<"程序执行完毕！"<<endl;
    return 0;
}