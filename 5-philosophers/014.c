#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct philo
{
    pthread_mutex_t _fork;
    int index;
    struct philo *next;
} philo;

philo *new_philo(int i)
{
    philo *guy;

    guy = malloc(sizeof(philo));
    guy->next = NULL;
    guy->index = i;
    if (pthread_mutex_init(&guy->_fork, NULL) != 0)
    {
        printf("Error creating mutex\n");
    }
    return (guy);
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
    int philos_len = 5;
    philo *main_philo = init_philosophers(philos_len);
    philo *curr = main_philo;
    int i = 0;
    while (i < philos_len)
    {
        if (pthread_create(&curr->_fork, NULL, eat, &curr) != 0)
            i++;
    }

    i = 0;
    while (i < philos_len)
    {
        printf("%d\n", main_philo->index);
        *main_philo = *(main_philo->next);
        i++;
    }

    return (0);
}