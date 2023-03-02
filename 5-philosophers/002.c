#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int n = 0;
pthread_mutex_t lock;
// int lock = PTHREAD_MUTEX_INITIALIZER;

void *func1(void *arg)
{
    pthread_mutex_lock(&lock);
    while (n < 5)
    {
        n++;
        usleep(500000);
        printf("func1, n=%d\n", n);
    }
    pthread_mutex_unlock(&lock);
    return (NULL);
}

void *func2(void *argc)
{
    pthread_mutex_lock(&lock);
    while (n > 0)
    {
        n--;
        usleep(500000);
        printf("func2, n=%d\n", n);
    }
    pthread_mutex_unlock(&lock);
    return (NULL);
}

int main(void)
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&thread1, NULL, func1, NULL);
    pthread_create(&thread2, NULL, func2, NULL);

    // dont't exit main thread
    // till children threads finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return (0);
}