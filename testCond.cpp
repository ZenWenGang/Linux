#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <pthread.h>

#define NUM 5
int cnt = 1000;

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER; // 定义锁
pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;   // 定义条件变量

void *threadrun(void *args)
{
    std::string name = static_cast<const char *>(args);
    while (true)
    {
        pthread_mutex_lock(&glock);
        //
        pthread_cond_wait(&gcond, &glock); // glock在pthread_cond_wait之前，会被自动释放掉
        std::cout << name << "计算:" << cnt << std::endl;
        cnt++;
        pthread_mutex_unlock(&glock);
    }
}

int main()
{
    std::vector<pthread_t> threads;
    for (int i = 0; i < NUM; i++)
    {
        pthread_t tid;
        char *name = new char[64];
        snprintf(name, 64, "thread-%d", i);
        int n = pthread_create(&tid, nullptr, threadrun, name);
        if (n != 0)
            continue;
        threads.push_back(tid);
        sleep(1);
    }
    sleep(3);
    // 每隔1s唤醒一个线程
    while (true)
    {
        std::cout << "唤醒所有线程..." << std::endl;
        pthread_cond_broadcast(&gcond);
        sleep(1);
    }
    for (auto &id : threads)
    {
        int m = pthread_join(id, nullptr);
        (void)m;
    }
    return 0;
}
