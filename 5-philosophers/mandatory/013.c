#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// timing
int time_to_eat = 100000;
int time_to_sleep = 100000;
int time_to_die = 110000;
int timing = 100000;
int number_of_philosophers = 30;
pthread_mutex_t lock;
pthread_mutex_t lock1;

typedef struct philo
{
    pthread_t thread;
    int death;

    int index;
    int num;
} philo;

philo new_philo(int i)
{
    philo var;
    var.index = i;
    var.num = 0;

    return (var);
}

void check(philo *arg)
{
    if (arg->death >= time_to_die)
    {
        printf("exit in %d\n", arg->index);
        exit(0);
    }
}

void *eat(void *arg)
{
    while (1)
    {
        usleep(timing); // time to wait to give other oportunity to take the lock
        // check((philo *)arg);
        pthread_mutex_lock(&lock);
        usleep(time_to_eat);
        // ((philo *)arg)->num++;
        printf("philo %d death: %d \n", ((philo *)arg)->index, ((philo *)arg)->death);
        ((philo *)arg)->death = 0;
        pthread_mutex_unlock(&lock);
    }
    return (NULL);
}

// void *sleeping(void *arg)
// {

// }

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
            // pthread_join(var[i].thread,NULL);
            var[i].death += time_to_sleep;
            usleep(time_to_sleep);
            // pthread_mutex_lock(&lock1);
            check(&var[i]);
            // pthread_mutex_unlock(&lock1);
            i++;
        }
    }

    return (0);
}