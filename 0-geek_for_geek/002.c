#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int global_var = 0;

void *Thread_func(void *thread_id_ptr)
{
    int *id = (int *)thread_id_ptr;
    static int static_var;

    // change both static and global variables
    global_var++;
    static_var++;

    printf("Thread id: %d, static: %d, globale: %d\n", *id, static_var, global_var);
    return (NULL);
}

int main(void)
{
    int i;
    pthread_t thread_id;

    // create 3 threads
    i = 0;
    while (i++ < 3)
        pthread_create(&thread_id, NULL, Thread_func, (void *)&thread_id);
    pthread_exit(NULL);
    return (0);
}