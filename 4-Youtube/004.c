#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void *func_a(void *arg)
{
    int *ptr = (int *)malloc(sizeof(int));
    *ptr = 5;
    int i = 0;
    while (i < 3)
    {
        sleep(1);
        printf("func_a -> %d -> %d\n", i, *ptr);
        (*ptr)++;
        i++;
    }
    return (ptr);
}

void *func_b(void *arg)
{
    int i = 0;
    while (i < 3)
    {
        sleep(2);
        printf("func_b -> %d\n", i);
        i++;
    }
    return (NULL);
}

int main(void)
{
    pthread_t thread;
    int j = 2;
    int *result;

    pthread_create(&thread, NULL, func_a, &j);
    func_b(NULL);

    // wait for specific thread
    pthread_join(thread, (void *)&result);
    printf("thread's done: j=%d, *result=%d\n", j, *result);
    return (0);
}