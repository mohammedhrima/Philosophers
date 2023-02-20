#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define FAILED != 0

void *print_hello(void *thread_id_void_ptr)
{
    int *thread_id;

    thread_id = (int *)thread_id_void_ptr;
    printf("Hello from thread %d\n", *thread_id);

    return (NULL);
}

int main(void)
{
    pthread_t thread_id;

    if (pthread_create(&thread_id, NULL, print_hello, (void *)&thread_id) FAILED)
    {
        printf("error creating thread\n");
        exit(1);
    }
    pthread_exit(NULL);
    return (0);
}