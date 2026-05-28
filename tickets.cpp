#include<iostream>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t mutex;//定义锁
int tickets=10;
 
void* worke(void* arg)
{
    const char* thread_name=(const char*)arg;
    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&mutex);
        if(tickets>0)
        {
            printf("%s卖出第%d张票\n",thread_name,tickets);
            tickets--;
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main()
{
    pthread_t t1,t2;

    pthread_mutex_init(&mutex,nullptr);//初始化锁
    
    pthread_create(&t1,nullptr,worke,(void*)"黄牛1");
    pthread_create(&t2,nullptr,worke,(void*)"黄牛2");

    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);

    pthread_mutex_destroy(&mutex);
    printf("所有票已售完，剩余票数：%d\n", tickets);
    return 0;
}

