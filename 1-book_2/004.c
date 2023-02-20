#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define WORK_SIZE 1024

int time_to_exit = 0;
char work_area[WORK_SIZE];
pthread_mutex_t mutex;

void *func(void *arg)
{
    sleep(50);
    pthread_mutex_lock(&mutex); // lock mutex
    printf("hello\n");
    pthread_mutex_unlock(&mutex); // unlock mutex

    return (NULL);
}

int main(void)
{
    int res;
    pthread_t thread_id;

    void *thread_ptr;
    int i = 0;

    // forks for spagetti
    // create mutex
    res = pthread_mutex_init(&mutex, NULL);
    if (res != 0)
    {
        printf("Error in creating mutex\n");
        exit(EXIT_FAILURE);
    }
    // create thread
    res = pthread_create(&thread_id, NULL, func, NULL);
    if (res != 0)
    {
        printf("Error in creating thread\n");
        exit(EXIT_FAILURE);
    }
}