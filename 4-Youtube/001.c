#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *func1(void *arg)
{
    while (1)
    {
        sleep(1);
        printf("func 1\n");
    }
    return (NULL);
}

void *func2(void *arg)
{
    while (1)
    {
        sleep(2);
        printf("func 2\n");
    }
    return (NULL);
}

int main(void)
{
    pthread_t thread;

    pthread_create(&thread, NULL, func1, NULL);
    func2(NULL);
    return (0);
}