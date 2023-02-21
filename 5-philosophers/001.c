#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *func1(void *arg)
{
    while (1)
    {
        usleep(500000);
        printf("hey from func 1\n");
    }
    return (NULL);
}

void *func2(void *argc)
{
    while (1)
    {
        usleep(500000);
        printf("hey from func 2\n");
    }
    return (NULL);
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, func1, NULL);
    pthread_create(&thread2, NULL, func2, NULL);

    // dont't exit main thread
    // till children threads finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return (0);
}