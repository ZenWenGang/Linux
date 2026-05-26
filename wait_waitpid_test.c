#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0)
        perror("fork "), exit(1);
    else if (pid == 0)
    {
        printf("i am child process\n");
        sleep(5);
        exit(0);
    }
    else
    {
        int st;
        pid_t wpid = waitpid(pid, &st, 0);
        printf("waitpid return: %d\n", wpid);

        if (WIFEXITED(st))
        {
            printf("子进程正常退出，退出码：%d\n", WEXITSTATUS(st));
        }
        else if (WIFSIGNALED(st))
        {
            printf("子进程被信号杀死，信号：%d\n", WTERMSIG(st));
            if (WCOREDUMP(st))
            {
                printf("子进程产生了core dump文件\n");
            }
            else
            {
                printf("子进程没有产生core dump文件\n");
            }
        }
        else
        {
            printf("子进程异常退出，状态未知\n");
        }
    }
    return 0;
}