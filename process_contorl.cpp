#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int main()
{
    //创建子进程
    pid_t pid;
    if((pid=fork())==-1)
    perror("fork"),exit(1);

    if(pid==0)
    {
        sleep(20);
        exit(10);
    }
    else//父进等待子进程
    {
        int st;
        int ret=wait(&st);
        if(ret>0&&(st&0X7F)==0)//情况1：wait成功 并且 低7位=0 → 正常退出
        {
            //正常退出
            printf("child exit code:%d\n",(st>>8)&0XFF);
        }
        else if(ret>0)//情况2：wait成功 并且 低7位≠0 → 异常（信号杀死）
        {
            //异常退出
            printf("sig code:%d\n",st& 0X7F);
        }
    }
    return 0;
}