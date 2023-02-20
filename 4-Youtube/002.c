#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *func1(void *arg)
{
    int i = 0;
    while (i< 3)
    {
        sleep(1);
        printf("func 1 -> %d\n", i);
        i++;
    }
    return (NULL);
}

void *func2(void *arg)
{
    int i = 0;
    while (i < 3)
    {
        sleep(2);
        printf("func 2 -> %d\n",i);
        i++;
    }
    return (NULL);
}

int main(void)
{
    pthread_t thread;

    pthread_create(&thread, NULL, func1, NULL);
    func2(NULL);
    pthread_join(thread, NULL);
    return (0);
}