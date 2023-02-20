#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define BIG 1000000000UL
uint32_t counter = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *count_to_big(void *arg)
{
    uint32_t i = 0;
    while (i < BIG)
    {
        pthread_mutex_lock(&lock);
        i++;
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return (NULL);
}

int main(void)
{
    pthread_t t;
    pthread_create(&t, NULL, count_to_big, NULL);
    count_to_big(NULL);
    // count_to_big(NULL);
    pthread_join(t, NULL);
    printf("Done. counter = %u\n", counter);
    return (0);
}