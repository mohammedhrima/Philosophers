#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

sem_t *my_semaphore;

int counter = 0;

void child_process()
{
    int i;
    for (i = 0; i < 5; i++)
    {
        sem_wait(my_semaphore); // Wait for the semaphore to be available
        counter++;              // Increment the shared counter
        printf("Child process %d: counter = %d\n", getpid(), counter);
        sem_post(my_semaphore); // Release the semaphore
        sleep(1);               // Sleep for 1 second to simulate some work being done
    }
}

int main()
{
    pid_t pid1, pid2;
    my_semaphore = sem_open("/my_semaphore", O_CREAT, 0644, 1);
    if (my_semaphore == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Create the first child process
    pid1 = fork();
    if (pid1 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid1 == 0)
    {
        // Child process 1
        child_process();
        exit(EXIT_SUCCESS);
    }

    // Create the second child process
    pid2 = fork();
    if (pid2 < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid2 == 0)
    {
        // Child process 2
        child_process();
        exit(EXIT_SUCCESS);
    }

    // Wait for the child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // Close and unlink the semaphore
    sem_close(my_semaphore);
    sem_unlink("/my_semaphore");

    return 0;
}
