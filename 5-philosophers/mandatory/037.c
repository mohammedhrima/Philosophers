#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// consts
#define THOUSAND 1000
#define MILLION 1000000

// structs
typedef struct timeval t_timing;
typedef pthread_mutex_t t_mutex;
typedef pthread_t t_thread;

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
    t_mutex number_of_philos_who_did_eat_mutex;

} t_shared;

typedef struct s_philo
{
    int index;

    time_t last_time_did_eat;
    t_thread thread;
    t_mutex fork_mutex;
    t_mutex last_time_did_eat_mutex;
    t_shared *data;

    struct s_philo *next;
} t_philo;

void my_sleep(time_t sleeping_timing)
{
    t_timing start;
    t_timing end;

    gettimeofday(&start, NULL);
    while (1)
    {
        usleep(5);
        gettimeofday(&end, NULL);
        if (end.tv_sec * THOUSAND + end.tv_usec / THOUSAND >= start.tv_sec * THOUSAND + start.tv_usec / THOUSAND + sleeping_timing)
            break;
    }
}

t_philo *new_philo(int index, t_shared *data)
{
    t_philo *philo;

    philo = malloc(sizeof(t_philo));
    philo->index = index + 1;
    pthread_mutex_init(&philo->fork_mutex, NULL);
    pthread_mutex_init(&philo->last_time_did_eat_mutex, NULL);
    philo->next = NULL;
    philo->data = data;

    return (philo);
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
    pthread_mutex_init(&data->printing_mutex, NULL);
    pthread_mutex_init(&data->number_of_philos_who_did_eat_mutex, NULL);
    return (data);
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

void take_fork(t_philo *philo, t_mutex *token_fork, int fork_index)
{
    t_timing current_time;

    pthread_mutex_lock(token_fork);
    gettimeofday(&current_time, NULL);
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d has taken fork %d\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index, fork_index);
    pthread_mutex_unlock(&philo->data->printing_mutex);
}

void eating(t_philo *philo)
{
    t_timing current_time;

    gettimeofday(&current_time, NULL);

    // check last time did eat
    pthread_mutex_lock(&philo->last_time_did_eat_mutex);
    philo->last_time_did_eat = current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time;

    pthread_mutex_lock(&philo->data->number_of_philos_who_did_eat_mutex);
    philo->data->number_of_philos_who_did_eat += 1;
    pthread_mutex_unlock(&philo->data->number_of_philos_who_did_eat_mutex);

    pthread_mutex_unlock(&philo->last_time_did_eat_mutex);

    // print state
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d is eating\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
    pthread_mutex_unlock(&philo->data->printing_mutex);

    // sleep eating time
    my_sleep(philo->data->time_to_eat);

    // unlock forks
    pthread_mutex_unlock(&philo->fork_mutex);
    pthread_mutex_unlock(&philo->next->fork_mutex);
}

void sleeping(t_philo *philo)
{
    t_timing current_time;

    // get time
    gettimeofday(&current_time, NULL);

    // print state
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d is sleeping\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
    pthread_mutex_unlock(&philo->data->printing_mutex);

    // sleep
    my_sleep(philo->data->time_to_sleep);
}

void thinking(t_philo *philo)
{
    t_timing current_time;

    // get time
    gettimeofday(&current_time, NULL);

    // print state
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d is thinking\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
    pthread_mutex_unlock(&philo->data->printing_mutex);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        take_fork(philo, &philo->fork_mutex, philo->index);
        take_fork(philo, &philo->next->fork_mutex, philo->next->index);
        eating(philo);
        sleeping(philo);
        thinking(philo);
    }
    return (NULL);
}

void check(t_philo *philo)
{
    t_timing current_time;
    while (1)
    {
        my_sleep(5);
        gettimeofday(&current_time, NULL);
        pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        pthread_mutex_lock(&philo->data->printing_mutex);
        if (current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time - philo->last_time_did_eat >= philo->data->time_to_die)
        {
            printf("%10lu: %d did died\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
            exit(0);
        }
        pthread_mutex_lock(&philo->data->number_of_philos_who_did_eat_mutex);
        if (philo->data->number_of_meals && philo->data->number_of_philos_who_did_eat / philo->data->number_of_meals == philo->data->number_of_philos)
            exit(0);
        pthread_mutex_unlock(&philo->data->number_of_philos_who_did_eat_mutex);

        pthread_mutex_unlock(&philo->data->printing_mutex);
        pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        // exit_if_all_philos_finished_eating(philo);
        philo = philo->next;
    }
}

// don't forget duk ltest dyul walu in atoi ...
// check pthread functions and gettimeofday if they failed
int main(void)
{
    int i;
    t_timing current_time;

    int number_of_philos = 5;
    time_t time_to_die = 800;
    time_t time_to_eat = 200;
    time_t time_to_sleep = 200;
    int number_of_meals = 2;

    // init shared data
    t_shared *data = new_shared(time_to_eat, time_to_die, time_to_sleep, number_of_meals, number_of_philos);
    // init philos
    t_philo *philo = init_philos(number_of_philos, data);

    // create threads
    gettimeofday(&current_time, NULL);
    data->starting_time = current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND;
    // data->starting_time = current_time.tv_sec * THOUSAND;

    i = 0;
    while (i < number_of_philos)
    {
        if (i % 2 == 0)
        {
            philo->last_time_did_eat = 0;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    my_sleep(10);
    i = 0;
    while (i < number_of_philos)
    {
        if (i % 2 == 1)
        {
            philo->last_time_did_eat = 0;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    // add check here
    check(philo);
}
