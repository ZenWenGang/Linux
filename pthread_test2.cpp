#include <iostream>
#include <pthread.h>
#include <stdlib.h>

void *worker(void *arg)
{
    int *p = (int *)malloc(sizeof(int));
    *p = 42;
    printf("工作线程准备退出，返回地址 %p 值为 %d\n", p, *p);
    pthread_exit(p); // free(p);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, worker, nullptr);

    void *ret;
    // 等待进程结束，并获取其返回值
    pthread_join(tid, &ret);

    printf("主线程获取到返回值：%d\n", *(int *)ret);
    free(ret); // 释放线程分配的内存
    return 0;
}