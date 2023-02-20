#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *func_a(void *arg)
{
    int *ptr = (int*)arg;
    int i = 0;
    while (i< 3)
    {
        sleep(1);
        printf("func_a -> %d -> %d\n", i, *ptr);
        (*ptr)++;
        i++;
    }
    return (NULL);
}

void *func_b(void *arg)
{
    int i = 0;
    while (i < 3)
    {
        sleep(2);
        printf("func_b -> %d\n",i);
        i++;
    }
    return (NULL);
}

int main(void)
{
    pthread_t thread;
    int j = 2;

    pthread_create(&thread, NULL, func_a,&j);
    func_b(NULL);
    //wait for specific thread
    pthread_join(thread, NULL);
    printf("thread's done: j=%d\n", j);
    return (0);
}