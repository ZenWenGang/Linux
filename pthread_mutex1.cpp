#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
int x = 10;

void *add(void *arg)
{
    pthread_mutex_lock(&mutex);
    x += 1;
    printf("线程%ld执行后，x = %d\n", (long)arg, x);
    pthread_mutex_unlock(&mutex); // 解锁：离开临界区
    return nullptr;
}

int main()
{
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, nullptr);

    pthread_create(&t1, nullptr, add, (void *)1);
    pthread_create(&t2, nullptr, add, (void *)2);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_mutex_destroy(&mutex); // 销毁互斥锁
    printf("最终x的值：%d\n", x);

    return 0;
}