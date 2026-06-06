#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

template <typename T>
class BlockingQueue
{
private:
    queue<T> _q;                  // 底层队列
    mutex _mtx;                   // 互斥锁
    condition_variable_not_full;  // 队列不满的条件变量
    condition_variable_not_empty; // 队列非空的条件变量
    size_t _max_size;             // 缓冲区最大容量

public:
    BlockingQueue(size_t max_size) : _max_size(max_size) {}

    // 生产者：在队列中放入数据
    void push(const T &data)
    {
        unique_lock<mutex> lock(_mtx);
        // while循环防止虚假唤醒
        while (_q.size() >= _max_size)
        {
            _not_full.wait(lock); // 队列满，生产者阻塞等待
        }
        _q.push(data);
        _not_empty.notify_one(); // 唤醒等待的消费者
    }
    // 消费者：从队列中取出数据
    T pop()
    {
        unique_lock<mutex> lock(_mtx);
        // while循环防止虚假唤醒
        while (_q.empty())
        {
            _not_empty.wait(lock); // 队列空，消费者阻塞等待
        }
        T data = _q.front();
        _q.push();
        _not_full.notify_one(); // 唤醒等待的生产者
        return data;
    }
};