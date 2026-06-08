#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "Log.hpp"
#include "Thread.hpp"
#include "Cond.hpp"
#include "Mutex.hpp"

namespace ThreadPoolModule
{
    using namespace ThreadModlue;
    using namespace LogModule;
    using namespace CondModule;
    using namespace MutexModule;

    static const int gnum = 5;
    template<typename T>
    class ThreadPool
    {
    public:
        ThreadPool(int num = gnum):_num(num)
        {
            for(int i = 0; i < num; i++)
            {
                _threads.emplace_back(
                    [this](){
                        HandlerTask();
                    }
                );
            }
        }
        void Start()
        {
            for(auto &thread : _threads)
            {
                thread.Start();
            }
        }
        void HandlerTask()
        {
            char name[128];
            pthread_getname_np(pthread_self(), name, sizeof(name));
            while(true)
            {
                T t;
                {
                    LockGuard lockguard(_mutex);
                    while(_taskq.empty())
                    {
                        _cond.Wait(_mutex);
                    }
                    //一定有任务
                    t = _taskq.front(); // 从q中获取任务，任务已经是线程私有的了！！！
                    _taskq.pop();
                }
                t(); // 处理任务，需要在临界区内部处理吗？1 0
            }
        }
        ~ThreadPool()
        {}
    private:
        std::vector<Thread> _threads;
        int _num; // 线程池中，线程的个数
        std::queue<T> _taskq;
        Cond _cond;
        Mutex _mutex;
    };
}