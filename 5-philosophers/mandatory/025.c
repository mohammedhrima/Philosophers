#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int time_to_sleep = 300000;
int time_to_eat = 100000;
int usleep_timing = 300000;
int time_to_die = 10000;

// struct
typedef struct timeval timing;
typedef struct s_philo
{
    int index;
    pthread_mutex_t _fork;
    pthread_t thread;
    timing last_time_did_eat;
    struct s_philo *next;
} t_philo;

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
        t_philo *var = (t_philo *)arg;
        if (pthread_mutex_lock(&var->_fork) == 0 && pthread_mutex_lock(&var->next->_fork) == 0)
        {
            gettimeofday(&var->last_time_did_eat, NULL);
            var->last_time_did_eat.tv_sec += (time_t)time_to_eat;
            // var->last_time_did_eat
            usleep(time_to_eat);
            printf("var[%d] is eating using %d and %d\n", var->index, var->index, var->next->index);
        }
        pthread_mutex_unlock(&var->_fork);
        pthread_mutex_unlock(&var->next->_fork);
        // printf("var[%d] is sleeping\n", var->index);
        usleep(time_to_sleep);
    }
    return (NULL);
}

// add a usleep here
void check(t_philo *var)
{
    t_philo *curr = var;
    timing current_time;
    while (1)
    {
        gettimeofday(&current_time, NULL);
        // add lock here
        if (current_time.tv_sec - var->last_time_did_eat.tv_sec >= (time_t)time_to_die)
        {
            printf("%d did died\n", var->index);
            exit(0);
        }
        curr = curr->next;
    }
}

int main(void)
{
    int len = 6;
    t_philo *philo = init_philos(len);
    printf("\n");
    int i = 0;
    while (i < len)
    {
        printf("-> %d\n", philo->index);
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
        philo->last_time_did_eat = current_time;
        pthread_create(&philo->thread, NULL, routine, philo);
        philo = philo->next;
        i++;
    }
    check(philo);
    i = 0;
}