#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// pthread_mutex_t philo_fork[2]; // it will be array half of number of philos
int time_to_eat = 0;

typedef struct philo
{
    int index;
    pthread_mutex_t lock_fork;
    int time_to_die;
    struct philo *next;
    // pthread_mutex_t mutex_fork;
} philo;

philo *new_philo(int i)
{
    philo *res;

    res = malloc(sizeof(philo));
    res->next = NULL;
    res->index = i;
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

int main(void)
{
    int philos_number = 10;
    int i = 0;
    philo *my_philo = build_linked_list(philos_number);
    while (i < philos_number + 2)
    {
        printf("47 -> %d\n", my_philo->index);
        my_philo = my_philo->next;
        i++;
    }
}
