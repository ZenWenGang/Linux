#include <iostream>
#include <unistd.h>
#include <pthread.h>

// 线程入口函数
void *thread_func(void *arg)
{
    int num = *(int *)arg; // 获取传入的参数
    std::cout << "线程 " << num << " 开始执行" << std::endl;
    sleep(1); // 模拟线程执行的时间
    std::cout << "线程 " << num << " 执行完成" << std::endl;
    return nullptr; // 线程函数必须返回一个 void* 类型的指针
}

int main()
{
    pthread_t tid1, tid2;
    int n1 = 1, n2 = 2;
    // 创建两个线程
    pthread_create(&tid1, NULL, thread_func, &n1);
    pthread_create(&tid2, NULL, thread_func, &n2);

    // 阻塞主线程，等待子线程执行完成
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    std::cout << "主线程退出" << std::endl;
    return 0;
}