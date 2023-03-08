#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int n = 50;

void *increment(void *arg)
{
    while (n < 100)
    {
        usleep(1);
        n++;
        printf("in: %d\n", n);
    }
    return (NULL);
}

void *decrement(void *arg)
{
    while (n > 0)
    {
        usleep(1);
        n--;
        printf("de: %d\n", n);
    }
    return (NULL);
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, decrement, NULL);
    while(1);
}