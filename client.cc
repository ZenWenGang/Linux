#include"comm.hpp"

int main()
{
    //获取共享内存和信号量
    int shmid=createShm();
    int semid=createSem();
    //映射共享内存到当前进程
    char* shmaddr=(char*)shmat(shmid,NULL,0);
    if(shmaddr==(char*)-1)
    {
        cerr<<"shmat error"<<endl;
        exit(1);
    }
    //写入数据到共享内存
    int cnt=0;
    while(true)
    {
        semP(semid);//加锁
        snprintf(shmaddr,SHM_SIZE,"Helo Shm! cnt:%d",cnt++);
        cout<<"Client send: "<<shmaddr<<endl;
        semV(semid);//解锁
        sleep(1);
    }
    //解除映射
    shmdt(shmaddr);
    return 0;
}