#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

typedef pthread_mutex_t t_mutex;

int counter = 0;

typedef struct s_shared
{
    time_t starting_time;
    // giving inputs
    time_t time_to_eat;
    time_t time_to_sleep;
    time_t time_to_die;

    int number_of_philos;
    int number_of_meals;
    int number_of_philos_who_did_eat;

    t_mutex printing_mutex;

} t_shared;

typedef struct s_philo
{
    int index;
    int number_of_meal_eaten;
    pid_t process_id;

    // t_shared *data;
    // semaphores
    sem_t *fork_semaphore;
    sem_t *printing_semaphore;

    struct s_philo *next;
} t_philo;
void routine(t_philo *philo)
{
    while (1)
    {
        usleep(80000);
        sem_wait(philo->fork_semaphore);
        sem_wait(philo->printing_semaphore);
        printf("%d is eating \n", philo->index);

        sem_post(philo->printing_semaphore);
        sem_post(philo->fork_semaphore);
    }
}

#define len 4

int main()
{
    t_philo philos[len];
    int i = 0;
    int number_of_philos = len;

    sem_t *semaphore = sem_open("/fork", O_CREAT, 0644, number_of_philos / 2 - number_of_philos % 2);
    sem_t *printing_semaphore = sem_open("/printing", O_CREAT, 0644, 1);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    while (i < len)
    {
        philos[i].index = i + 1;
        philos[i].number_of_meal_eaten = 0;
        philos[i].process_id = fork();
        philos[i].fork_semaphore = semaphore;
        philos[i].printing_semaphore = printing_semaphore;
        if (philos[i].process_id == 0)
        {
            /// child process
            // printf("process %d is created \n", philos[i].index);
            routine(philos + i);
            exit(0);
            // while(1);
        }
        else
        {
            i++;
            // parent process
        }
    }

    i = 0;
    while (i < len)
    {
        i++;
        wait(NULL);
    }

#if 0
    pid_t pid1, pid2;

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
#endif
    // Close and unlink the semaphore
    sem_close(semaphore);
    sem_unlink("/fork");
    sem_unlink("/printing");

    return 0;
}
