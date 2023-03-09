#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

typedef pthread_mutex_t t_mutex;

int counter = 0;
time_t time_to_eat = 300000;

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

    t_shared *data;
    struct s_philo *next;
} t_philo;

void routine(t_philo *philo)
{
    // printf("%d started the routine\n", philo->index);
    while (1)
    {
        // usleep(3000);
        // printf("%d is waiting for fork\n", philo->index);
        // usleep()
        // sem_wait(philo->fork_semaphore);
        sem_wait(philo->printing_semaphore);
        printf("%d is eating meal %d \n", philo->index, philo->number_of_meal_eaten);
        usleep(time_to_eat);
        philo->number_of_meal_eaten++;
        if (philo->number_of_meal_eaten == 4)
        {
            printf("philo %d did finish eating 4 meals\n", philo->index);
            // exit(0);
            break;
        }
        sem_post(philo->printing_semaphore);
        // sem_post(philo->fork_semaphore);
        // usleep(10);
    }
}

#define len 6

int main()
{
    t_philo philos[len];
    int i = 0;
    int number_of_philos = len;

    // sem_t *fork_semaphore = sem_open("/fork", O_CREAT, 0644, 3);
    sem_t *fork_semaphore = sem_open("/fork", O_CREAT, 0644, number_of_philos);
    sem_t *printing_semaphore = sem_open("/printing", O_CREAT, 0644, 1);

    if (fork_semaphore == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    while (i < len)
    {

        philos[i].process_id = fork();
        if (philos[i].process_id == 0)
        {
            // printf("philo %d in process %d\n", i, philos[i].process_id);
            philos[i].index = i + 1;
            philos[i].number_of_meal_eaten = 0;
            philos[i].fork_semaphore = fork_semaphore;
            philos[i].printing_semaphore = printing_semaphore;
            /// child process
            // printf("process %d is created \n", philos[i].index);
            // usleep(5);
            routine(philos + i);
            exit(0);
            // while(1);
        }
        else
            i++;
        }
    wait(NULL);
    // Close and unlink the semaphore
    sem_close(fork_semaphore);
    sem_close(printing_semaphore);
    sem_unlink("/fork");
    sem_unlink("/printing");

    return 0;
}
