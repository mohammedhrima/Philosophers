#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int time_to_sleep = 100000;
int time_to_eat = 100000;
int usleep_timing = 300000;
int time_to_die = 10000;

// struct
typedef struct timeval timing;
typedef struct s_philo
{
    int index;
    pthread_t thread;
    timing last_time_did_eat;
    pthread_mutex_t fork_mutex;
    pthread_mutex_t last_time_did_eat_mutex;
    pthread_mutex_t *printing_mutex;
    struct s_philo *next;
} t_philo;

t_philo *new_philo(int index)
{
    t_philo *res = malloc(sizeof(t_philo));
    res->index = index;
    res->next = NULL;
    pthread_mutex_init(&res->fork_mutex, NULL);
    // pthread_mutex_init(&res->last_time_did_eat_mutex, NULL);
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
        t_philo *var = (t_philo *)arg;
        usleep(usleep_timing);
        if (pthread_mutex_lock(&var->fork_mutex) == 0 && pthread_mutex_lock(&var->next->fork_mutex) == 0)
        {
            gettimeofday(&var->last_time_did_eat, NULL);
            var->last_time_did_eat.tv_sec += (time_t)time_to_eat;
            // var->last_time_did_eat
            usleep(2 * usleep_timing);
            pthread_mutex_lock(var->printing_mutex);
            printf("philosopher %d has taken fork %d and %d\n", var->index, var->index, var->next->index);
            // pthread_mutex_unlock(var->printing_mutex);

            usleep(usleep_timing);
            // pthread_mutex_lock(var->printing_mutex);
            printf("philosopher %d is eating\n", var->index);
            // pthread_mutex_unlock(var->printing_mutex);
            usleep(time_to_eat);
        }
        pthread_mutex_unlock(&var->fork_mutex);
        pthread_mutex_unlock(&var->next->fork_mutex);
        // lock printing
        usleep(usleep_timing);
        // pthread_mutex_lock(var->printing_mutex);
        printf("philosopher %d is sleeping\n", var->index);
        pthread_mutex_unlock(var->printing_mutex);
        usleep(time_to_sleep);
    }
    return (NULL);
}

// add a usleep here
void check(t_philo *var)
{
    // t_philo *curr = var;
    // timing current_time;
    while (1)
    {
        // gettimeofday(&current_time, NULL);
        // // add lock here
        // if (current_time.tv_sec - var->last_time_did_eat.tv_sec >= (time_t)time_to_die)
        // {
        //     printf("%d did died\n", var->index);
        //     exit(0);
        // }
        // curr = curr->next;
    }
}

int main(void)
{
    int len = 6;
    t_philo *philo = init_philos(len);
    pthread_mutex_t *printing_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(printing_mutex, NULL);

    printf("\n");
    int i = 0;
    while (i < len)
    {
        philo->printing_mutex = printing_mutex;
        // printf("-> %d\n", philo->index);
        philo = philo->next;
        i++;
    }
    printf("\n");
    // inti timing
    timing current_time;
    gettimeofday(&current_time, NULL);

    i = 0;
    while (i < len)
    {
        // if (i % 2 == 0)
        // {
        philo->last_time_did_eat = current_time;
        pthread_create(&philo->thread, NULL, routine, philo);
        // }
        philo = philo->next;
        i++;
    }
    // usleep(2*usleep_timing);
    // i = 0;
    // while (i < len)
    // {
    //     if (i % 2 == 1)
    //     {
    //         philo->last_time_did_eat = current_time;
    //         pthread_create(&philo->thread, NULL, routine, philo);
    //     }
    //     philo = philo->next;
    //     i++;
    // }
    check(philo);
    // i = 0;
}