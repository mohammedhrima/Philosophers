#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int counter = 0;
sem_t *my_semaphore;

typedef struct
{
    int index;
    pthread_t thread;
    int *ptr;
    sem_t *semaphore;
} var;

void *inc_function(void *arg)
{
    int i;
    var *my_var = (var *)arg;
    while (1)
    {
        usleep(500000);
        sem_wait(my_var->semaphore);         // Wait for the semaphore to be available
        *(my_var->ptr) = *(my_var->ptr) + 1; // Increment the shared counter
        printf("Thread %d, i = %d\n", my_var->index, *(my_var->ptr));
        sem_post(my_var->semaphore); // Release the semaphore
    }
    pthread_exit(NULL);
}

void *dec_function(void *arg)
{
    int i;
    var *my_var = (var *)arg;
    while (1)
    {
        usleep(500000);
        sem_wait(my_var->semaphore);         // Wait for the semaphore to be available
        *(my_var->ptr) = *(my_var->ptr) - 1; // Increment the shared counter
        printf("Thread %d, i = %d\n", my_var->index, *(my_var->ptr));
        sem_post(my_var->semaphore); // Release the semaphore
    }
    pthread_exit(NULL);
}


int main()
{

    var my_var[2];
    int i = 0;
    //////////////////////
    my_var[0].index = 0;
    my_var[1].index = 1;
    ////////////////////////
    my_var[0].ptr = &i;
    my_var[1].ptr = &i;

    sem_t *semaphore;

    int result;
    ///////////////////////
    my_var[0].semaphore = semaphore;
    my_var[1].semaphore = semaphore;
    ///////////////////////
    // my_semaphore = sem_open("/my_semaphore", O_CREAT, 0644, 1);
    semaphore = sem_open("/my_semaphore", O_CREAT, 0644, 1);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Create the threads
    result = pthread_create(&my_var[0].thread, NULL, inc_function, my_var);
    if (result != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    result = pthread_create(&my_var[1].thread, NULL, dec_function, my_var + 1);
    if (result != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Wait for the threads to finish
    pthread_join(my_var[0].thread, NULL);
    pthread_join(my_var[1].thread, NULL);

    // Close and unlink the semaphore
    sem_close(my_var[0].semaphore);
    sem_close(my_var[1].semaphore);

    sem_unlink("/my_semaphore");

    return 0;
}
