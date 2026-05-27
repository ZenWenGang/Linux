#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *countdown_task(void *arg)
{
    printf("分离线程开始倒计时...\n");
    
    for (int i = 3; i > 0; i--)
    {
        printf("倒计时：%d秒\n", i);
        sleep(1);
    }

    printf("倒计时结束，分离线程退出，资源自动回收\n");
    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, countdown_task, nullptr);
    pthread_detach(tid);

    printf("主线程继续运行，无需等待线程结束\n");
    sleep(4); // 让主线程晚一点退出，确保分离线程能执行完
    printf("主线程退出\n");

    return 0;
}