#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define failed != 0

void *func(void *arg)
{
    printf("running func from %d\n", *((int *)arg));
    return (NULL);
}

int main()
{
    pthread_t thread_id[5];
    int r;
    int n = 10;
    int i = 0;

    printf("running threads...\n");
    while (i < 2)
    {
        r = pthread_create(thread_id + i, NULL, func, (void *)&n);
        if (r failed)
        {
            perror("Error creating thread\n");
            exit(1);
        }
        int ptr;
        // pthread_join(thread_id[i], (void *)&ptr);
        i++;
    }
    pthread_exit(NULL);
    return (0);
}