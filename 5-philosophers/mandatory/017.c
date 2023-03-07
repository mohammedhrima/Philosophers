#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int timing = 300000;
int time_to_eat = 300000;
int time_to_sleep = 400000;

typedef struct philo
{
    pthread_mutex_t _fork;
    pthread_t thread;
    int index;
    struct philo *next;
} philo;

int n = 0;
// pthread_mutex_t lock;

void *eat(void *arg)
{
    philo *curr = (philo *)arg;
    while (1)
    {
        usleep(time_to_eat);
        printf("philo %d with address %p is eating \n", curr->index, curr);

    }
    return (NULL);
}

philo *new_philo(int index)
{
    philo *man;

    man = malloc(sizeof(philo));
    memset(man, 0, sizeof(philo));
    man->index = index;

    man->next = NULL;
    if (pthread_mutex_init(&man->_fork, NULL) != 0)
    {
        printf("Error creating mutex\n");
        exit(-1);
    }
    printf("philo %d is created\n", index); // if something went wrong check wth address
    return (man);
}

philo *init_philosophers(int len)
{
    philo *head;
    philo *curr;

    head = new_philo(0);
    curr = head;
    int i = 1;
    while (i < len)
    {
        curr->next = new_philo(i);
        curr = curr->next;
        i++;
    }

    curr->next = head;
    return (head);
}

int main(void)
{
    // pthread_mutex_init(&lock, NULL);

    int len = 6;
    int i;
    printf("\n");
    // initialize philosophers
    philo *philos = init_philosophers(len);

    // initialize array of threads
    // pthread_t *array = malloc(len * sizeof(pthread_t));
    printf("\n");

    philo *curr = philos;
    i = 0;
    // create threads
    // while (i < len)
    // {
    //     // if (i % 2 == 0)
    //     // {
    //     printf("create thread for philo %d\n", curr->index);
    //     // if (pthread_create(&curr->thread, NULL, eat, curr) != 0)
    //     // {
    //     //     printf("Error creating thread\n");
    //     //     exit(-1);
    //     // };
    //     // }
    //     curr = curr->next;
    //     i++;
    // }
    i = 0;
    // create threads
    // while (i < len)
    // {
    //     if (i % 2 == 1)
    //     {
    //         printf("create thread for philo %d\n", curr->index);
    //         if (pthread_create(&curr->thread, NULL, eat, curr) != 0)
    //         {
    //             printf("Error creating thread\n");
    //             exit(-1);
    //         };
    //     }
    //     curr = curr->next;
    //     i++;
    // }
    printf("\n");
    i = 0;
    while (i < len)
    {
        printf("index: %d\n", curr->index);
        curr = curr->next;
        i++;
    }
    printf("\n");

    // start checking
    while (1)
    {
        i = 0;
        while (i < len)
        {
            if (curr->index % 2 == 2)
            {
                usleep(timing);
            }
            pthread_mutex_lock(&curr->_fork);
            pthread_mutex_lock(&curr->next->_fork);
            usleep(timing);
            pthread_mutex_lock(&curr->next->_fork);
            pthread_mutex_lock(&curr->_fork);
            curr = curr->next;
            // printf("\n=============================================================\n");
            i++;
        }
        // i = 0;
        // while (i < len)
        // {
        //     if (i % 2 == 1)
        //         usleep(timing);
        //     // printf("\n=============================================================\n");
        //     i++;
        // }
    }

    return (0);
}