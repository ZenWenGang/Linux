#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

int shared_counter = 0;
bool done = false;

// 线程函数：多个线程有序对计数器自增
void* increment(void* arg) {
    while (true) {
        // 1. 加锁
        pthread_mutex_lock(&mutex);

        // 2. 循环检查条件：如果计数器已满或已完成，退出循环
        while (done || shared_counter >= 100) {
            if (done || shared_counter >= 100) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            // 条件不满足，等待被唤醒（原子释放锁并阻塞）
            pthread_cond_wait(&cond, &mutex);
        }

        // 3. 条件满足：计数器自增并打印
        shared_counter++;
        printf("%s拿到了锁，当前计数器：%d\n", (char*)arg, shared_counter);

        // 4. 唤醒其他等待线程，让它们也有机会执行
        pthread_cond_signal(&cond);

        // 5. 如果计数器到100，标记完成并广播唤醒所有线程
        if (shared_counter == 100) {
            done = true;
            pthread_cond_broadcast(&cond);
        }

        // 6. 解锁
        pthread_mutex_unlock(&mutex);

        // 模拟耗时操作，让其他线程有机会抢到锁
        usleep(100);
    }
}

int main() {
    pthread_t t1, t2, t3, t4;

    // 创建4个工作线程
    pthread_create(&t1, NULL, increment, (void*)"线程1");
    pthread_create(&t2, NULL, increment, (void*)"线程2");
    pthread_create(&t3, NULL, increment, (void*)"线程3");
    pthread_create(&t4, NULL, increment, (void*)"线程4");

    // 等待所有线程执行完毕
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    // 打印最终结果
    printf("\nFinal counter: %d\n", shared_counter);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}


































