#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>

// consts
#define THOUSAND 1000
#define MILLION 1000000

// structs
typedef struct timeval t_timing;
typedef pthread_mutex_t t_mutex;
typedef pthread_t t_thread;

int counter = 0;
time_t time_to_eat = 500000;

// structs
typedef struct s_shared
{
    time_t starting_time;
    // giving inputs
    time_t time_to_eat;
    time_t time_to_sleep;
    time_t time_to_die;

    // // // semaphores
    // sem_t *fork_semaphore;
    // sem_t *printing_semaphore;

    int number_of_philos;
    int number_of_meals;
    int number_of_philos_who_did_eat;

} t_shared;

typedef struct s_philo
{
    int index;
    int number_of_meal_eaten;
    pid_t process_id;

    time_t last_time_did_eat;
    t_mutex last_time_did_eat_mutex;

    // sem_t *fork_semaphore;
    // sem_t *printing_semaphore;

    t_shared *data;
    // struct s_philo *next;
} t_philo;

///////////////////////////////////////////////////////////////////////////////////////////
void my_sleep(time_t sleeping_timing)
{
    t_timing start;
    t_timing end;

    if (gettimeofday(&start, NULL) != 0)
    {
        printf("Error 62\n");
        exit(-1);
    }
    while (1)
    {
        usleep(5);
        if (gettimeofday(&end, NULL) != 0)
        {
            printf("Error 70\n");
            break;
        }
        if (end.tv_sec * THOUSAND + end.tv_usec / THOUSAND >= start.tv_sec * THOUSAND + start.tv_usec / THOUSAND + sleeping_timing)
            break;
    }
}

t_shared *new_shared(time_t time_to_eat, time_t time_to_die, time_t time_to_sleep, int number_of_meals, int number_of_philos)
{
    t_shared *data;

    data = malloc(sizeof(t_shared));

    data->time_to_eat = time_to_eat;
    data->time_to_sleep = time_to_sleep;
    data->time_to_die = time_to_die;
    data->number_of_meals = number_of_meals;
    data->number_of_philos = number_of_philos;
    data->number_of_philos_who_did_eat = 0;

    return (data);
}

t_philo *new_philo(int index, t_shared *data)
{
    t_philo *philo;

    philo = malloc(sizeof(t_philo));
    philo->index = index + 1;
    philo->number_of_meal_eaten = 0;
    // philo->next = NULL;
    philo->data = data;

    return (philo);
}

t_philo **init_philos(int number_of_philos, t_shared *data)
{
    int i = 0;
    t_philo **philos = malloc(number_of_philos * sizeof(t_philo *));
    while (i < number_of_philos)
    {
        philos[i] = new_philo(i, data);
        i++;
    }
    return (philos);
}

// void sleeping(t_philo *philo)
// {
//     t_timing current_time;

//     // get time
//     if (gettimeofday(&current_time, NULL) != 0)
//     {
//         printf("Error\n");
//         exit(-1);
//     }
//     // print state
//     printf("%10lu: %d is sleeping\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
//     // sleep
//     my_sleep(philo->data->time_to_sleep);
// }

// void thinking(t_philo *philo)
// {
//     t_timing current_time;

//     // get time
//     if (gettimeofday(&current_time, NULL) != 0)
//     {
//         printf("Error\n");
//         exit(-1);
//     }
//     // print state
//     // pthread_mutex_lock(&philo->data->printing_mutex);
//     printf("%10lu: %d is thinking\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
//     // pthread_mutex_unlock(&philo->data->printing_mutex);
// }

// void take_fork(t_philo *philo, sem_t *printing_semaphore, sem_t *fork_semaphore)
// {
//     t_timing current_time;

//     // pthread_mutex_lock(token_fork);
//     if (gettimeofday(&current_time, NULL) != 0)
//     {
//         printf("Error\n");
//         exit(-1);
//     }
//     sem_wait(fork_semaphore);
//     sem_wait(printing_semaphore);
//     printf("%10lu: %d has taken a fork\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
//     sem_wait(printing_semaphore);
//     sem_wait(fork_semaphore);
// }

void routine(t_philo *philo, sem_t *printing_semaphore, sem_t *fork_semaphore)
{
    t_timing current_time;

    while (1)
    {
        // check if les than max meals
        if (philo->data->number_of_meals == -1 || philo->number_of_meal_eaten < philo->data->number_of_meals)
        {
            // printf("philo %d, number of meals eaten %d, total number of meals %d\n", philo->index, philo->number_of_meal_eaten, philo->data->number_of_meals);
            my_sleep(5);
            sem_wait(fork_semaphore);
            // printf("184\n");
            if (gettimeofday(&current_time, NULL) != 0)
            {
                printf("Error\n");
                exit(-1);
            }
            sem_wait(printing_semaphore);
            printf("%10lu: %d has taken a fork 1\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
            sem_post(printing_semaphore);

            if (gettimeofday(&current_time, NULL) != 0)
            {
                printf("Error\n");
                exit(-1);
            }
            sem_wait(printing_semaphore);
            printf("%10lu: %d has taken a fork 2\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
            sem_post(printing_semaphore);

            if (gettimeofday(&current_time, NULL) != 0)
            {
                printf("Error\n");
                exit(-1);
            }
            sem_wait(printing_semaphore);
            printf("%10lu: %d is eating %d \n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index, philo->number_of_meal_eaten);
            sem_post(printing_semaphore);

            my_sleep(philo->data->time_to_eat);
            // add ine if data->number_of_meals is more than -1
            // in dying block printing with sem wait
            if (philo->data->number_of_meals != -1)
                philo->number_of_meal_eaten++;
            if (philo->number_of_meal_eaten == philo->data->number_of_meals)
            {
                sem_post(fork_semaphore);
                sem_wait(printing_semaphore);
                printf("philo %d did finish eating\n", philo->index);
                break;
            }
            sem_post(fork_semaphore);

            if (gettimeofday(&current_time, NULL) != 0)
            {
                printf("Error\n");
                exit(-1);
            }
            // sleeping
            sem_wait(printing_semaphore);
            printf("%10lu: %d is sleeping \n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
            sem_post(printing_semaphore);
            my_sleep(philo->data->time_to_sleep);

            if (gettimeofday(&current_time, NULL) != 0)
            {
                printf("Error\n");
                exit(-1);
            }
            // thinking
            sem_wait(printing_semaphore);
            printf("%10lu: %d is thinking \n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
            sem_post(printing_semaphore);
        }
    }
}

// check orphan processes
int main()
{
    sem_unlink("/fork");
    sem_unlink("/printing");
    // t_philo philos[len];
    int i = 0;

    t_timing current_time;
    int number_of_philos = 2;
    time_t time_to_die = 400;
    time_t time_to_eat = 200;
    time_t time_to_sleep = 200;
    // handle if there is no number of meals
    // if not giving set it with -1
    int number_of_meals = 5;
    ////////////////////////
    sem_t *fork_semaphore = sem_open("/fork", O_CREAT, 0644, number_of_philos);
    sem_t *printing_semaphore = sem_open("/printing", O_CREAT, 0664, number_of_philos);

    if (!fork_semaphore || !printing_semaphore)
    {
        printf("Error 171\n");
        exit(-1);
    }

    // init shared data
    t_shared *data = new_shared(time_to_eat, time_to_die, time_to_sleep, number_of_meals, number_of_philos);
    // init philos
    t_philo **philo = init_philos(number_of_philos, data);

    i = 0;
    while (i < data->number_of_philos)
    {
        // printf("line 187\n");
        // printf("line 189\n");
        my_sleep(1);
        philo[i]->process_id = fork();
        if (philo[i]->process_id == 0)
        {
            if (gettimeofday(&current_time, NULL) != 0)
            {
                printf("Error 178\n");
                exit(-1);
            }
            philo[i]->data->starting_time = current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND;
            // sem_wait(exit_semaphore);
            routine(philo[i], fork_semaphore, printing_semaphore);
            // sem_post(exit_semaphore);
            // post exit_sem then exit child process form here
            exit(0);
        }
        else
            i++;
    }
    // first ended process will let the main kill all child processes
    wait(NULL);

    i = 0;
    while (i < data->number_of_philos)
    {
        kill(philo[i]->process_id, SIGKILL);
        // waitpid(philos[j].process_id, NULL, 0);
        i++;
    }
    // Close and unlink the semaphore
    sem_close(fork_semaphore);
    sem_close(printing_semaphore);
    sem_unlink("/fork");
    sem_unlink("/printing");

    return 0;
}
