#include <pthread.h>
#include <stdio.h>

void *func(void *arg)
{
    int *num = (int *)arg;
    printf("hello from func: %d\n", *num);
    (*num)++;
    return (NULL);
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;
    int x = 1;

    pthread_create(&thread1, NULL, func, (void *)&x);
    pthread_create(&thread2, NULL, func, (void *)&x);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return (0);
}