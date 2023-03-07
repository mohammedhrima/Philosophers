#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// timing
int time_to_eat = 100;
int time_to_sleep = 150000;
int time_to_die = 50;
int timing = 3000;
int number_of_philosophers = 5;
pthread_mutex_t lock;

typedef struct philo
{
    pthread_t thread;

    int index;
    int num;
} philo;

philo new_philo(int i)
{
    philo var;
    var.index = i;
    var.num = 0;
    // if (pthread_create(&var.thread, NULL, action, &var) != 0)
    // {
    //     printf("error creating thread\n");
    //     exit(0);
    // }
    return (var);
}

void *eat(void *arg)
{
    while (1)
    {
        usleep(5000);
        pthread_mutex_lock(&lock);
        usleep(time_to_sleep);
        ((philo *)arg)->num++;
        printf("philo %d doing action to num: %d \n", ((philo *)arg)->index, ((philo *)arg)->num);
        pthread_mutex_unlock(&lock);
    }
    return (NULL);
}

void check(philo *arg)
{
    if (arg->num > 100)
    {
        printf("exit in %d\n", arg->index);
        exit(0);
    }
}

int main(void)
{
    philo *var;

    pthread_mutex_init(&lock, NULL);
    int i = 0;
    var = malloc(number_of_philosophers * sizeof(philo));
    while (i < number_of_philosophers)
    {
        var[i] = new_philo(i);
        i++;
    }
    i = 0;
    while (i < number_of_philosophers)
    {
        if (pthread_create(&var[i].thread, NULL, eat, &var[i]) != 0)
        {
            printf("error creating thread\n");
            exit(0);
        }
        i++;
    }
    while (1) // stop if philo a died
    // verify sleeping
    {
        i = 0;
        while (i < number_of_philosophers)
        {
            // pthread_join(&var[i - 1] )
            usleep(time_to_sleep);
            check(&var[i]);
            i++;
        }
    }

    return (0);
}