#include <iostream>
#include <mutex>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "Mutex.hpp"

using namespace MutexModule;

int ticket = 1000;
class ThreadData
{
public:
    ThreadData(const std::string &n, Mutex &lock)
        : name(n),
          lockp(&lock) {}
    ~ThreadData() {}
    std::string name;
    Mutex *lockp;
};

// 加锁：尽量加锁的范围粒度要比较细，尽可能的不要包含太多的非临界区代码
void *route(void *arg)
{
    ThreadData *td = static_cast<ThreadData *>(arg);
    while (1)
    {
        LockGuard guard(*td->lockp); // 加锁完成，RAII风格的互斥锁的实现
        if (ticket > 0)
        {
            usleep(1000);
            printf("%s sells ticket:%d\n", td->name.c_str(), ticket);
            ticket--;
        }
        else
        {
            break;
        }
        usleep(123);
    }
    return nullptr;
}

int main()
{
    {
        int a = 10;
    }
    int a = 20;

    Mutex lock;
    pthread_t t1, t2, t3, t4;
    ThreadData *td1 = new ThreadData("thread 1", lock);
    pthread_create(&t1, nullptr, route, td1);

    ThreadData *td2 = new ThreadData("thread 2", lock);
    pthread_create(&t2, nullptr, route, td2);

    ThreadData *td3 = new ThreadData("thread 3", lock);
    pthread_create(&t3, nullptr, route, td3);

    ThreadData *td4 = new ThreadData("thread 4", lock);
    pthread_create(&t4, nullptr, route, td4);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    return 0;
}