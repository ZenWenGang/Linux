// 阻塞队列的实现
#pragma once

#include <iostream>
#include <string>
#include <queue>
#include "Mutex.hpp"
#include "Cond.hpp"

const int defaultcap = 10; // for test

using namespace MutexModule;
using namespace CondModule;

template <typename T>
class BlockQueue
{
private:
    bool IsFull() { return _q.size() >= _cap; }
    bool IsEmpty() { return _q.empty(); }

public:
    BlockQueue(int cap = defaultcap)
        : _cap(cap), _csleep_num(0), _psleep_num(0)
    {
    }
    void Equeue(const T &in)
    {
        {
            LockGuard lockguard(_mutex);
            // 生产者调用
            while (IsFull())
            {
                // 应该让生产者线程进行等待
                // 重点1：pthread_cond_wait调用成功，挂起当前线程之前，要先自动释放锁！！
                // 重点2：当线程被唤醒的时候，默认就在临界区内唤醒！要从pthread_cond_wait
                // 成功返回，需要当前线程，重新申请_mutex锁！！！
                // 重点3：如果我被唤醒，但是申请锁失败了？？我就会在锁上阻塞等待！！！
                _psleep_num++;
                std::cout << "生产者，进入休眠了: _psleep_num" << _psleep_num << std::endl;
                // 问题1: pthread_cond_wait是函数吗？有没有可能失败？pthread_cond_wait立即返回了
                // 问题2：pthread_cond_wait可能会因为，条件其实不满足，pthread_cond_wait 伪唤醒
                _full_cond.Wait(_mutex);
                _psleep_num--;
            }
            // 100%确定：队列有空间
            _q.push(in);

            // 临时方案
            // v2
            if (_csleep_num > 0)
            {
                _empty_cond.Signal();
                std::cout << "唤醒消费者..." << std::endl;
            }
        }
    }
    T Pop()
    {
        T data;
        {
            // 消费者调用
            LockGuard lockguard(_mutex);

            while (IsEmpty())
            {
                _csleep_num++;
                _empty_cond.Wait(_mutex);
                _csleep_num--;
            }
            data = _q.front();
            _q.pop();

            if (_psleep_num > 0)
            {
                _full_cond.Signal();
                std::cout << "唤醒生产者" << std::endl;
            }
        }
        return data;
    }
    ~BlockQueue()
    {
    }

private:
    std::queue<T> _q; // 临界资源！！！
    int _cap;         // 容量大小

    Mutex _mutex;
    Cond _full_cond;
    Cond _empty_cond;

    int _csleep_num; // 消费者休眠的个数
    int _psleep_num; // 生产者休眠的个数
};