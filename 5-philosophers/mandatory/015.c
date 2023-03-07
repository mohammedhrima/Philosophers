#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct philo
{
    pthread_mutex_t _fork;
    pthread_t thread;
    int index;
    struct philo *next;
} philo;

int n = 0;
pthread_mutex_t lock;
void *eat(void *arg)
{
    // philo *curr;
    // curr = (philo *)arg;
    // printf("%d is eating \n", ((philo *)arg)->index);
    while (1)
    {
        // usleep(500000);
        // pthread_mutex_lock(&((philo *)arg)->_fork);
        usleep(500000);
        printf("%d is eating \n", ((philo *)arg)->index);
        n = 5;
        // pthread_mutex_unlock(&((philo *)arg)->_fork);
        usleep(500000);
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
    printf("philo %d is created\n", index);
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
    pthread_mutex_init(&lock, NULL);

    int len = 5;
    int i;
    printf("\n");
    // initialize philosophers
    philo *philos = init_philosophers(len);

    // initialize array of threads
    pthread_t *array = malloc(len * sizeof(pthread_t));
    printf("\n");

    philo *curr = philos;
    i = 0;
    // create threads
    while (i < len)
    {
        printf("create thread for philo %d\n", curr->index);
        if (pthread_create(array + i, NULL, eat, &curr) != 0)
        {
            printf("Error creating thread\n");
            exit(-1);
        };
        // curr->thread = array[i];
        curr = curr->next;
        i++;
    }
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
        // usleep(500000);
        // int j = 0;
        // while (j < len)
        // {
        //     curr = curr->next;
        // }
    }

    return (0);
}