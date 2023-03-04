#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define THOUSAND 1000
#define MILLION 1000000
useconds_t usleep_timing = 0;

// struct
// typedef struct timeval t_timing;
typedef pthread_mutex_t t_mutex;
typedef pthread_t t_thread;
typedef unsigned long t_micro_sec;

typedef struct s_philo
{
    t_micro_sec last_time_did_eat;
    t_thread thread;
    // mutexes
    t_mutex last_time_did_eat_mutex;
    t_mutex fork_mutex;
    int index;
    struct s_philo *next;
} t_philo;

typedef struct s_shared_data
{
    // giving inputs
    t_micro_sec time_to_eat;
    t_micro_sec time_to_die;
    t_micro_sec time_to_sleep;
    int number_of_philos;
    int number_of_meals;
    // timing
    t_micro_sec printing_timing;
    // conditions for exiting
    int all_alive;
    int all_philos_did_eat_number_of_meals;
    // mutexes
    t_mutex *printing_mutex;
    t_mutex checking;
    // pointer to the head of philos
    t_philo *philo;
} t_shared_data;

t_micro_sec current_time(void)
{
    struct timeval *now;

    now = malloc(sizeof(struct timeval));
    gettimeofday(now, NULL);
    return (now->tv_sec * MILLION + now->tv_usec);
}

t_philo *new_philo(int index)
{
    t_philo *philo;

    philo = malloc(sizeof(t_philo));
    philo->index = index + 1;
    philo->next = NULL;
    pthread_mutex_init(&philo->fork_mutex, NULL);
    pthread_mutex_init(&philo->last_time_did_eat_mutex, NULL);
    printf("new philo %d\n", philo->index);
    return (philo);
}

t_philo *init_philos(int number_of_philos)
{
    int i = 1;
    t_philo *curr = new_philo(0);
    t_philo *head = curr;
    while (i < number_of_philos)
    {
        curr->next = new_philo(i);
        curr = curr->next;
        i++;
    }
    curr->next = head;
    return (head);
}

#if 0
void my_sleep(t_micro_sec sleeping_timing)
{
    t_micro_sec start;

    usleep(sleeping_timing);

    // start = current_time();
    // printf("sleep %lu\n", sleeping_timing);
    // printf("enter my sleep go from %lu -> %lu\n", start, start + sleeping_timing);
    // while ((start + sleeping_timing) / THOUSAND > current_time() / THOUSAND)
    // {
    //     // printf(" in while loop\n");
    //     ;
    // }
    // printf("exit my sleep\n");
}
#endif

void print_state(t_shared_data *table, t_philo *philo, char *str)
{
    pthread_mutex_lock(table->printing_mutex);
    // printf("%lu: %d %s\n", current_time(), philo->index, str);
    printf("%d %s\n", philo->index, str);
    pthread_mutex_unlock(table->printing_mutex);
}

void *routine(void *arg)
{
    t_shared_data *table = (t_shared_data *)arg;
    t_philo philo = *(table->philo);
    while (1)
    {
        // printf("in routine\n");
        if (pthread_mutex_lock(&philo.fork_mutex) == 0 && pthread_mutex_lock(&philo.next->fork_mutex) == 0)
        {
            pthread_mutex_lock(&philo.last_time_did_eat_mutex);
            philo.last_time_did_eat = current_time() + table->time_to_eat;
            pthread_mutex_unlock(&philo.last_time_did_eat_mutex);
            //////////////////////////////////////////////////////
            // take a fork
            print_state(table, &philo, "has taken fork");
            print_state(table, &philo, "is eating");
            usleep(table->time_to_eat);
        }
        pthread_mutex_unlock(&philo.fork_mutex);
        pthread_mutex_unlock(&philo.next->fork_mutex);
        print_state(table, &philo, "is sleeping");
        usleep(table->time_to_sleep);
        print_state(table, &philo, "is thinking");
    }
    return (NULL);
}

void check(t_shared_data *table)
{
    while (1)
        ;
}

int main(void)
{
    // don't forget duk ltest dyul walu in atoi ...
    // check pthread functions and gettimeofday if they failed
    // init table
    int i;
    t_shared_data *table = malloc(sizeof(t_shared_data));
    // init mutexes
    table->printing_mutex = malloc(sizeof(t_mutex));
    pthread_mutex_init(table->printing_mutex, NULL);
    // pthread_mutex_init(&table->all_alive, NULL);
    pthread_mutex_init(&table->checking, NULL);
    // init timings
    table->time_to_sleep = 300000;
    table->time_to_eat = 100000;
    table->time_to_die = 600000;
    table->number_of_philos = 6;
    table->number_of_meals = 0;
    table->philo = init_philos(table->number_of_philos);
    //////////////
    // t_mutex lock;
    // pthread_mutex_init(&lock, NULL);
    i = 0;
    while (i < table->number_of_philos)
    {
        if (i % 2 == 0)
        {
            // pthread_mutex_lock(&table->philo->last_time_did_eat_mutex);
            table->philo->last_time_did_eat = current_time();
            pthread_create(&table->philo->thread, NULL, routine, table);
            // pthread_mutex_unlock(&table->philo->last_time_did_eat_mutex);
        }
        table->philo = table->philo->next;
        i++;
    }
    i = 0;
    while (i < table->number_of_philos)
    {
        if (i % 2 == 1)
        {
            // pthread_mutex_lock(&table->last_time_did_eat_mutex);
            table->philo->last_time_did_eat = current_time();
            pthread_create(&table->philo->thread, NULL, routine, table);
            // pthread_mutex_unlock(&table->last_time_did_eat_mutex);
        }
        table->philo = table->philo->next;
        i++;
    }
    check(table);
}
