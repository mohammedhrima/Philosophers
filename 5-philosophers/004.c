#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int TIME_TO_LIVE = 1000 * 1;
int TIME_TO_EAT = 1000000 * 2;
int TIME_TO_SLEEP = 1000000 * 2;
// int NUMBER_OF_MEALS = 500000; // use it after

typedef struct philo
{
    int index;
    pthread_t thread;
    pthread_mutex_t fork_mutex;
    pthread_mutex_t time_to_live_mutex;
    pthread_mutex_t read_thread_mutex;
    int time_to_live;
    struct philo *next;
    // pthread_mutex_t mutex_fork;
} philo;

philo *new_philo(int i)
{
    philo *res;

    res = malloc(sizeof(philo));
    res->next = NULL;
    res->index = i;
    res->time_to_live = TIME_TO_LIVE;
    // if(pthread_create(res->thread))
    // {

    // }
    if (pthread_mutex_init(&res->fork_mutex, NULL) != 0)
    {
        printf("Error in creating fork mutex in philo %d\n", res->index);
        exit(-1);
    }
    if (pthread_mutex_init(&res->time_to_live_mutex, NULL) != 0)
    {
        printf("Error in creating time to live mutex in philo %d\n", res->index);
        exit(-1);
    }
    if (pthread_mutex_init(&res->read_thread_mutex, NULL) != 0)
    {
        printf("Error in creating read mutex in philo %d\n", res->index);
        exit(-1);
    }
    return (res);
}

philo *build_linked_list(int len)
{
    int i = 1;
    philo *head;
    head = new_philo(0);
    philo *curr = head;
    while (i < len)
    {
        curr->next = new_philo(i);
        curr = curr->next;
        i++;
    }
    curr->next = head;
    return (head);
}

void *eat(void *arg)
{
    philo *curr = (philo *)arg;
    if (curr->time_to_live > 0 && pthread_mutex_lock(&curr->fork_mutex) == 0 && pthread_mutex_lock(&curr->next->fork_mutex) == 0)
    {
        if (pthread_mutex_lock(&curr->time_to_live_mutex) == 0)
        {
            if (curr->time_to_live <= 0)
            {
                printf("philo %d died\n", curr->index);
            }
            curr->time_to_live = TIME_TO_LIVE;
            pthread_mutex_unlock(&curr->time_to_live_mutex);
        }
        // else
        // {
        //     printf("Error in line 79\n");
        // }

        usleep(TIME_TO_EAT);
        printf("philo %d is eating\n", curr->index);

        // leaves the forks after eating
        pthread_mutex_unlock(&curr->fork_mutex);
        pthread_mutex_unlock(&curr->next->fork_mutex);
    }
    else
    {
        // leaves the forks befor sleeping
        // pthread_mutex_unlock(&curr->fork_lock);
        // pthread_mutex_unlock(&curr->next->fork_lock);

        // wait for them
        // pthread_join()

        if (pthread_mutex_lock(&curr->time_to_live_mutex) == 0)
        {
            curr->time_to_live--;
            if (curr->time_to_live <= 0)
            {
                printf("philo %d died\n", curr->index);
            }
            pthread_mutex_unlock(&curr->time_to_live_mutex);
        }

        usleep(TIME_TO_SLEEP);
        printf("philo %d is sleeping\n", curr->index);
    }
    return (NULL);
}

int main(void)
{
    int philos_number = 5;
    int i = 0;
    philo *my_philo = build_linked_list(philos_number);
    while (i < philos_number) // be carefull
    {
        printf("philo %d\n", my_philo->index);
        my_philo = my_philo->next;
        i++;
    }
    printf("\n======================================================\n");
    while (1)
    {
        if (pthread_mutex_lock(&my_philo->read_thread_mutex) == 0)
        {
            pthread_create(&my_philo->thread, NULL, eat, my_philo);
            my_philo = my_philo->next;
            pthread_mutex_unlock(&my_philo->read_thread_mutex);
        }
    }
}
