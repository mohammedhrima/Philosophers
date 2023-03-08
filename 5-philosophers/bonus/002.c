#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int counter = 0;
sem_t *my_semaphore;

void *thread_function(void *arg)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        sem_wait(my_semaphore); // Wait for the semaphore to be available
        counter++;              // Increment the shared counter
        printf("Thread %ld: counter = %d\n", pthread_self(), counter);
        sem_post(my_semaphore); // Release the semaphore
    }
    pthread_exit(NULL);
}



int main()
{
    pthread_t thread1, thread2;
    int result;

    my_semaphore = sem_open("/my_semaphore", O_CREAT, 0644, 1);
    if (my_semaphore == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Create the threads
    result = pthread_create(&thread1, NULL, thread_function, NULL);
    if (result != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    result = pthread_create(&thread2, NULL, thread_function, NULL);
    if (result != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Wait for the threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Close and unlink the semaphore
    sem_close(my_semaphore);
    sem_unlink("/my_semaphore");

    return 0;
}
