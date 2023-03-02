#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct philo
{
    pthread_mutex_t _fork;
    pthread_t thread;
    int index;
    struct philo *next;
} philo;

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

void *eat(void *arg)
{
    return (NULL);
}

int main(void)
{
    int len = 5;
    philo *philos = init_philosophers(len);
    philo *curr = philos;
    int i = 0;
    while (i < len)
    {
        if (pthread_create(&curr->thread, NULL, eat, &curr) != 0)
        {
            printf("Error creating thread\n");
            exit(-1);
        };
        curr = curr->next;
        i++;
    }
    i = 0;
    curr = philos;
    while (i < len)
    {
        printf("%d\n", curr->index);
        curr = curr->next;
        i++;
    }
    return(0);
}