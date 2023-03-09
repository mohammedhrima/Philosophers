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

    sem_t *fork_semaphore;
    sem_t *printing_semaphore;

    t_shared *data;
    struct s_philo *next;
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
    philo->next = NULL;
    philo->data = data;

    return (philo);
}

t_philo *init_philos(int number_of_philos, t_shared *data)
{
    int i = 1;
    t_philo *curr = new_philo(0, data);
    curr->next = curr;
    t_philo *head = curr;
    while (i < number_of_philos)
    {
        curr->next = new_philo(i, data);
        curr = curr->next;
        i++;
    }
    curr->next = head;
    return (head);
}

void routine(t_philo *philo)
{
    while (1)
    {
        // usleep(3000);
        // check if les than max meals
        if (philo->data->number_of_meals == -1 || philo->number_of_meal_eaten < philo->data->number_of_meals)
        {
            // printf("philo %d, number of meals eaten %d, total number of meals %d\n", philo->index, philo->number_of_meal_eaten, philo->data->number_of_meals);
            sem_wait(philo->fork_semaphore);
            sem_wait(philo->printing_semaphore);
            printf("%d take a fork\n", philo->index);
            printf("%d take a fork\n", philo->index);
            printf("%d is eating %d \n", philo->index, philo->number_of_meal_eaten);
            usleep(time_to_eat);
            // add ine if data->number_of_meals is more than -1
            philo->number_of_meal_eaten++;
            if (philo->number_of_meal_eaten == philo->data->number_of_meals)
            {
                printf("philo %d did finish eating\n", philo->index);
                break;
            }
            sem_post(philo->printing_semaphore);
            sem_post(philo->fork_semaphore);
        }
    }
}

// check orphan processes
int main()
{
    // t_philo philos[len];
    int i = 0;

    t_timing current_time;
    int number_of_philos = 4;
    time_t time_to_die = 400;
    time_t time_to_eat = 200;
    time_t time_to_sleep = 200;
    // handle if there is no number of meals
    // if not giving set it with -1
    int number_of_meals = 5;
    ////////////////////////
    sem_t *fork_semaphore = sem_open("/forkk", O_CREAT, 0644, number_of_philos / 2);
    sem_t *printing_semaphore = sem_open("/printingg", O_CREAT, 0664, 1);

    if (!fork_semaphore || !printing_semaphore)
    {
        printf("Error 171\n");
        exit(-1);
    }

    // init shared data
    t_shared *data = new_shared(time_to_eat, time_to_die, time_to_sleep, number_of_meals, number_of_philos);
    // init philos
    t_philo *philo = init_philos(number_of_philos, data);

    i = 0;
    while ( i < philo->data->number_of_philos)
    {
        philo->fork_semaphore = fork_semaphore;
        philo->printing_semaphore = printing_semaphore;
        i++;
    }
    i = 0;

    // printf("line 176\n");
    if (gettimeofday(&current_time, NULL) != 0)
    {
        printf("Error 178\n");
        exit(-1);
    }
    data->starting_time = current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND;

    i = 0;
    while (i < data->number_of_philos)
    {
        // printf("line 187\n");
        my_sleep(10);
        // printf("line 189\n");
        philo->process_id = fork();
        if (philo->process_id == 0)
        {
            // printf("line 193\n");
            // sem_wait(exit_semaphore);
            routine(philo);
            // sem_post(exit_semaphore);
            // post exit_sem then exit child process form here
            exit(0);
        }
        else
        {
            philo = philo->next;
            i++;
        }
    }
    // first ended process will let the main kill all child processes
    wait(NULL);
#if 0
    int j = 0;
    while (j < len)
    {
        waitpid(philos[j].process_id, NULL, 0);
        j++;
    }
#else
    i = 0;
    while (i < data->number_of_philos)
    {
        kill(philo->process_id, SIGKILL);
        philo = philo->next;
        // waitpid(philos[j].process_id, NULL, 0);
        i++;
    }
#endif
    // Close and unlink the semaphore
    sem_close(philo->fork_semaphore);
    sem_close(philo->printing_semaphore);
    sem_unlink("/fork");
    sem_unlink("/printing");
    sem_unlink("/forkk");
    sem_unlink("/printingg");

    return 0;
}
