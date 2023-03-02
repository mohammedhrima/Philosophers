#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int time_to_sleep = 300000;
int time_to_eat = 900000;
int timing = 300000;

// struct
typedef struct s_philo
{
    int index;
    int c;
    pthread_mutex_t _fork;
    pthread_t thread;
    struct s_philo *next;
} t_philo;

t_philo *new_philo(int index)
{
    t_philo *res = malloc(sizeof(t_philo));
    res->c = 0;
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

void *func1(void *arg)
{
    while (1)
    {
        t_philo *var = (t_philo *)arg;
        if (pthread_mutex_lock(&var->_fork) == 0 && pthread_mutex_lock(&var->next->_fork) == 0)
        {
            usleep(time_to_eat);
            printf("var[%d] is eating using %d and %d\n", var->index, var->index, var->next->index);
        }
        pthread_mutex_unlock(&var->_fork);
        pthread_mutex_unlock(&var->next->_fork);
        usleep(timing);
    }
    return (NULL);
}

void *func2(void *arg)
{
    while (1)
    {
        t_philo *var = (t_philo *)arg;
        var = var->next;
    }
    return (NULL);
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
    // printf("=> %d\n", philo->index);
    // exit(0);
    i = 0;
    while (i < len)
    {
        pthread_create(&philo->thread, NULL, func1, philo);
        philo = philo->next;
        i++;
    }

    while (1)
        ;
    i = 0;
}