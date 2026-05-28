#include <pthread.h>
#include <stdio.h>
#include<iostream>
int x=10;
pthread_mutex_t mutex;

void* add_x(void* arg)
{
    //临界区内加锁
    pthread_mutex_lock(&mutex);
    //临界区内操作共享资源
    printf("线程%ld获取到锁，开始操作x\n",(long)arg);
    x-=1;
    printf("线程%ld操作后，x=%d\n",(long)arg,x);
    //操作完成后解锁
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main()
{
    pthread_t t1,t2;
    //初始化互斥锁
    pthread_mutex_init(&mutex,nullptr);

    //创建两个线程
    pthread_create(&t1,nullptr,add_x,(void*)1);
    pthread_create(&t2,nullptr,add_x,(void*)2);
 
    //等待线程
    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);

    //销毁互斥锁
    pthread_mutex_destroy(&mutex);
    std::cout<<"最终的x值："<<x<<std::endl;
    return 0;
}














