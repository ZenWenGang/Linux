#ifndef __COMM_HPP__
#define __COMM_HPP__

#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <cstring>

using namespace std;

// 生成key用的路径和项目ID
#define PATHNAME "./comm.hpp"
#define PROJ_ID 0x6666

// 共享内存大小
#define SHM_SIZE 4096

// 信号量操作函数
union semun {
    int val;
};

// P操作（加锁）
void semP(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

// V操作（解锁）
void semV(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

// 创建/获取信号量
int createSem()
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        cerr << "semget error" << endl;
        exit(1);
    }
    // 初始化信号量为1
    union semun su;
    su.val = 1;
    semctl(semid, 0, SETVAL, su);
    return semid;
}

// 创建/获取共享内存
int createShm()
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        cerr << "shmget error" << endl;
        exit(1);
    }
    return shmid;
}

#endif












































