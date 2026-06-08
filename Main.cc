#include "Log.hpp"
#include "ThreadPool.hpp"
#include <memory>

using namespace LogModule;
using namespace ThreadPoolModule;

int main()
{
    Enable_Console_Log_Strategy();
    ThreadPool<int> *tp = new ThreadPool<int>();
    tp->Start();
    sleep(100);
    return 0;
}