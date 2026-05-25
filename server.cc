#include "comm.hpp"

int main()
{
    // 1. 创建共享内存和信号量
    int shmid = createShm();
    int semid = createSem();

    // 2. 映射共享内存到当前进程
    char* shmaddr = (char*)shmat(shmid, NULL, 0);
    if (shmaddr == (char*)-1)
    {
        cerr << "shmat error" << endl;
        exit(1);
    }

    // 3. 循环读取共享内存
    while (true)
    {
        semP(semid); // 加锁
        if (strlen(shmaddr) > 0)
        {
            cout << "Server recv: " << shmaddr << endl;
            memset(shmaddr, 0, SHM_SIZE); // 读完清空
        }
        semV(semid); // 解锁
        sleep(1);
    }

    // 4. 解除映射（实际不会走到这里）
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    return 0;
}