#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_counter = 0;

void *increment(void *arg)
{
    const char *thread_name = (const char *)arg;
    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&mutex);
        printf("%s拿到了锁\n", thread_name); // 打印拿锁的线程
        shared_counter++;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, (void *)"线程A");
    pthread_create(&t2, NULL, increment, (void *)"线程B");

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    printf("最终计数: %d\n", shared_counter);
    return 0;
}