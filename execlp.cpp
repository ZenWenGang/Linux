#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(pid==0)
    {
        printf("子进程：即将执行 ls -la 命令\n");
        int ret=execlp("ls","ls","-l","-a",nullptr);

        if(ret==-1)
        {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("父进程：等待子进程执行 ls 命令\n");
        int wstatus;
        waitpid(pid,&wstatus,0);
        printf("父进程：子进程执行完毕，退出状态：%d\n", WEXITSTATUS(wstatus));
    }
    return 0;
}