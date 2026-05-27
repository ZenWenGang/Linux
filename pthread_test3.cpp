#include <pthread.h>
#include <stdio.h>
#include <cstdlib>
void *multiply(void *arg)
{
    int *nums = (int *)arg;
    int *result = (int *)malloc(sizeof(int));
    *result = nums[0] * nums[1];
    return result;
}

int main()
{
    pthread_t tid;
    int nums[] = {6, 7};
    pthread_create(&tid, nullptr, multiply, nums);

    void *ret;

    pthread_join(tid, &ret);
    printf("计算结果：%d × %d = %d\n", nums[0], nums[1], *(int *)ret);
    free(ret);
    return 0;
}