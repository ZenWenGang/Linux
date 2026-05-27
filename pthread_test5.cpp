#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

pid_t get_kernel_tid()
{
    return syscall(SYS_gettid);
}

void* thread_func(void* arg)
{
    pthread_t user_tid=pthread_self();
    pid_t kernel_tid=get_kernel_tid();
    printf("子线程 - 用户态ID: %lu, 内核态ID: %d\n", user_tid, kernel_tid);
    sleep(1);
    return nullptr;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, thread_func, nullptr);

    pthread_t main_user_tid = pthread_self();
    pid_t main_kernel_tid = get_kernel_tid();
    printf("主线程 - 用户态ID: %lu, 内核态ID(进程PID): %d\n", main_user_tid, main_kernel_tid);
    pthread_join(tid, NULL);
    return 0;
}