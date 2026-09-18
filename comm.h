#pragma once
#include<iostream>
#include<string>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#define MAXMUN 1024
#define PATH "."
#define FIFONAME "fifo"
using namespace std;

class Fifo
{
    public:
    Fifo(string path=PATH,string fifoname=FIFONAME)
    {
        _path=path;
        _fifoname=fifoname;
        _path_name=_path+"/"+_fifoname;
        if(access(_path_name.c_str(),F_OK)==-1)
        {
            //创建管道
            if(mkfifo(_path_name.c_str(),0666))
            cerr<<"mkfifo"<<endl;
        }
    }

    string get_path()
    {
        return _path;
    }

    string get_path_name()
    {
        return _path_name;
    }

    ~Fifo()
    {
        unlink(_path_name.c_str());
    }
    private:
    string _path;
    string _fifoname;
    string _path_name;
};

//封装命名管道接口
class NamePipe
{
    public:
    NamePipe(string path=PATH,string pipe_name=FIFONAME)
    {
        _path_name=path+"/"+pipe_name;
    }

    int WriteForFifo()
    {
        if(fd!=-1)
            close(fd);
        fd=open(_path_name.c_str(),O_WRONLY,0666);  
        if(fd==-1)
            cerr<<"fdfd打开失败"<<endl;
        return fd;   
    }

    int ReadForFifo()
    {
        fd=open(_path_name.c_str(),O_RDONLY,0666);
        if(fd==-1)
            cerr<<"fd打开失败"<<endl;
        return fd;
    }

    int get_fd()
    {
        return fd;
    }

    string Read()
    {
        char buffer[MAXMUN]={0};
        read(fd,buffer,sizeof(buffer));
        string str(buffer);
        return str;
    }

    bool Write(string str)
    {
        int n=write(fd,str.c_str(),str.size());
        if(n==-1)
        {
            cerr<<"write写入失败"<<endl;
            return false;
        }
        return true;
    }

    ~NamePipe()
    {
        if(fd!=-1)
        close(fd);
    }

    private:
    string _path_name;
    int fd=-1;
};