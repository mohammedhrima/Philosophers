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
typedef unsigned long t_micro_sec;

typedef struct s_shared
{
    t_micro_sec time_to_print;
    // giving inputs
    t_micro_sec time_to_eat;
    t_micro_sec time_to_die;
    t_micro_sec time_to_sleep;
    int number_of_philos;
    int number_of_meals;

    // conditions for exiting
    int all_alive;
    int all_philos_did_eat_number_of_meals;

    t_mutex printing_mutex;
    t_mutex checking_mutex;

} t_shared;

typedef struct s_philo
{
    int index;

    t_micro_sec last_time_did_eat;
    t_thread thread;

    t_mutex fork_mutex;
    t_mutex last_time_did_eat_mutex;

    int did_finish_all_meals;
    t_shared *data;

    struct s_philo *next;
} t_philo;

t_micro_sec current_time(void)
{
    struct timeval *now;

    now = malloc(sizeof(struct timeval));
    gettimeofday(now, NULL);
    // tv_sec: seconds
    // tv_usec: micro_seconds
    return (now->tv_sec * MILLION + now->tv_usec);
}

void my_sleep(t_micro_sec sleeping_timing) // takes argument in milisecond
{
    t_micro_sec end;
    t_micro_sec curr;

    curr = current_time();
    end = curr + sleeping_timing * THOUSAND; // in micro seconds
    while (curr < end)
        curr = current_time();
}

t_philo *new_philo(int index, t_shared *data)
{
    t_philo *philo;

    philo = malloc(sizeof(t_philo));
    philo->index = index + 1;

    // init mutexes
    pthread_mutex_init(&philo->fork_mutex, NULL);
    pthread_mutex_init(&philo->last_time_did_eat_mutex, NULL);

    philo->did_finish_all_meals = 0;
    philo->next = NULL;

    philo->data = data;

    printf("new philo %d\n", philo->index);

    return (philo);
}

t_shared *new_shared(t_micro_sec time_to_eat, t_micro_sec time_to_die, t_micro_sec time_to_sleep)
{
    t_shared *data;

    data = malloc(sizeof(t_shared));

    // init timings
    data->time_to_eat = time_to_eat;
    data->time_to_die = time_to_die;
    data->time_to_sleep = time_to_sleep;

    // init mutexes
    pthread_mutex_init(&data->printing_mutex, NULL);
    pthread_mutex_init(&data->checking_mutex, NULL);

    // init exit conditions
    data->all_alive = 1;
    data->all_philos_did_eat_number_of_meals = 0;

    return (data);
}

t_philo *init_philos(int number_of_philos, t_shared *data)
{
    int i = 1;
    t_philo *curr = new_philo(0, data);
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

int ft_strcmp(char *string1, char *string2)
{
    int i;

    i = 0;
    while (string1 && string2 && string1[i] && string1[i] == string2[i])
        i++;
    return (string1[i] - string2[i]);
}

void print_state(t_philo *philo, char *str)
{
    // pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%lu: %d %s", current_time(), philo->index, str);
    if (ft_strcmp(str, "has taken fork") == 0)
        printf(" %d %d\n", philo->index, philo->next->index);
    // pthread_mutex_unlock(&philo->data->printing_mutex);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    while (1)
    {
        // lock for checking ?
        // pthread_mutex_lock(&philo->fork_mutex);
        // pthread_mutex_lock(&philo->next->fork_mutex);

        // take a fork
        print_state(philo, "has taken fork");
        // pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        philo->last_time_did_eat = current_time() + philo->data->time_to_eat;
        // pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        // start eating
        print_state(philo, "is eating\n");
        my_sleep(philo->data->time_to_eat);

        // unlock forks after eating
        // pthread_mutex_unlock(&philo->next->fork_mutex);
        // pthread_mutex_unlock(&philo->fork_mutex);

        print_state(philo, "is sleeping\n");
        my_sleep(philo->data->time_to_sleep);
        print_state(philo, "is thinking\n");
    }
    return (NULL);
}

void check(t_philo *philo)
{
    t_micro_sec time_to_eat = philo->data->time_to_eat;
    while (1)
    {
        // pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        if (current_time() - time_to_eat > philo->last_time_did_eat)
        {
            print_state(philo, "did died\n");
            exit(0);
        }
        // pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        philo = philo->next;
    }
}

// don't forget duk ltest dyul walu in atoi ...
// check pthread functions and gettimeofday if they failed
int main(void)
{
    int i;

    // giving variables from input
    // convert them to micro seconds
    t_micro_sec time_to_sleep = 200000 / THOUSAND;
    t_micro_sec time_to_eat = 100000 / THOUSAND;
    t_micro_sec time_to_die = 800000 / THOUSAND;
    // t_micro_sec time_to_print = 1000;
    int number_of_philos = 2;

    // init shared data
    t_shared *data = new_shared(time_to_eat, time_to_die, time_to_sleep);
    // init philos
    t_philo *philo = init_philos(number_of_philos, data);

    printf("\n\n");

    // create threads
    i = 0;
    while (i < number_of_philos)
    {
        if (i % 2 == 0)
        {
            philo->last_time_did_eat = current_time();
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    i = 0;
    while (i < number_of_philos)
    {
        if (i % 2 == 1)
        {
            philo->last_time_did_eat = current_time();
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    // add check here
    check(philo);
}
