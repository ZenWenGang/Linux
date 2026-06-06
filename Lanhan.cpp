#include <iostream>
#include <mutex>
#include <pthread.h>

pthread_mutex_t mtx;

class test
{
private:
    test(int a, int b)
        : _a(a),_b(b)
    {}
    test(const test&)=delete;
public:
    static test* get_instance()
    {
        if(_inst==nullptr)
        {
            pthread_mutex_lock(&mtx);//加锁
            if(_inst==nullptr)
            _inst=new test(1,2);
            pthread_mutex_unlock(&mtx);
        }
        return _inst;
    }
    void print()
    {
        printf("a:%d,b:%d\n",_a,_b);
    }

private:
    int _a;
    int _b;
    static test *_inst;
};

test* test::_inst=nullptr;

int main()
{
    pthread_mutex_init(&mtx, nullptr);
    test::get_instance()->print();
    return 0;
}
