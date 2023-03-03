#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

// struct
typedef struct timeval timing;

typedef struct s_philo
{
    timing last_time_did_eat;
    pthread_mutex_t _fork;
    pthread_t thread;
    int index;
    struct s_philo *next;
} t_philo;

typedef struct s_table
{
    time_t time_to_die;
    time_t time_to_eat;
    time_t time_to_sleep;
    timing current_time;
    int someone_died;
    pthread_mutex_t printing_lock;
    t_philo *philo;
} t_table;

t_philo *new_philo(int index)
{
    t_philo *res = malloc(sizeof(t_philo));
    res->index = index;
    res->next = NULL;
    pthread_mutex_init(&res->_fork, NULL);
    printf("new philo %d\n", res->index);
    return (res);
}

t_philo *init_philos(int len)
{
    int i = 1;
    t_philo *curr = new_philo(0);
    t_philo *head = curr;
    while (i < len)
    {
        curr->next = new_philo(i);
        curr = curr->next;
        i++;
    }
    curr->next = head;
    return (head);
}

void *routine(void *arg)
{
    while (1)
    {
        t_table *table = (t_table *)arg;
        t_philo *philo = table->philo;
        usleep(100000);
        if (pthread_mutex_lock(&philo->_fork) == 0 && pthread_mutex_lock(&philo->next->_fork) == 0)
        {
            gettimeofday(&philo->last_time_did_eat, NULL);
            philo->last_time_did_eat.tv_sec += table->time_to_eat;
            pthread_mutex_lock(&table->printing_lock);
            printf("var[%d] is eating using %d and %d\n", philo->index, philo->index, philo->next->index);
            pthread_mutex_unlock(&table->printing_lock);
            usleep(table->time_to_eat);
        }
        pthread_mutex_unlock(&philo->_fork);
        pthread_mutex_unlock(&philo->next->_fork);
        pthread_mutex_lock(&table->printing_lock);
        printf("var[%d] is sleeping\n", philo->index);
        pthread_mutex_unlock(&table->printing_lock);
        usleep(table->time_to_sleep);
    }
    return (NULL);
}

// add a usleep here
void check(t_table *table)
{
    // t_philo *curr = table->philo;
    timing current_time;
    while (1)
    {

        gettimeofday(&current_time, NULL);
        // add lock here
        if (current_time.tv_sec - table->philo->last_time_did_eat.tv_sec >= table->time_to_die)
        {
            printf("%d did died\n", table->philo->index);
            exit(0);
        }
        table->philo = table->philo->next;
    }
}

int main(void)
{
    int len = 5;
    t_table *table;
    int i;

    // init table
    table = malloc(sizeof(t_table));
    table->time_to_sleep = 300000;
    table->time_to_eat = 300000;
    table->time_to_die = 10000;
    pthread_mutex_init(&table->printing_lock, NULL);

    // init philos
    table->philo = init_philos(len);

    // check philos
    i = 0;
    t_philo *philo = table->philo;
    while (i < len)
    {
        printf("-> %d\n", philo->index);
        philo = philo->next;
        i++;
    }

    // init timing
    gettimeofday(&table->current_time, NULL);
    // create threads
    i = 0;
    while (i < len)
    {
        table->philo->last_time_did_eat.tv_sec = table->current_time.tv_sec; // verify if I should add micro seconds
        pthread_create(&table->philo->thread, NULL, routine, table);
        table->philo = table->philo->next;
        i++;
    }
    check(table);
}