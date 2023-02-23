#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// timing
int time_to_eat = 100;
int time_to_sleep = 3000;
int time_to_die = 100;
int number_of_philosophers = 5;
int timing = 3000;

typedef struct philo
{
    pthread_t thread;
    pthread_mutex_t lock;
    int index;
    int num;
} philo;

void *eat(void *arg)
{
    // pthread_mutex_lock(&((philo*)arg)->lock);
    philo *s;
    s = (philo *)arg;
    usleep(timing);
    ((philo *)arg)->num++;
    printf("philo %d doing action to num: %d \n", s->index, s->num);
    // pthread_mutex_unlock(&((philo*)arg)->lock);

    return (NULL);
}

philo new_philo(int i)
{
    philo var;
    var.index = i;
    var.num = 0;
    printf("%d\n", var.index);
    if (pthread_mutex_init(&var.lock, NULL) != 0)
    {
        printf("Error in creating thread\n");
        exit(0);
    }
    // usleep(100);
    // var.index = 0;

    return (var);
}

void check(philo *arg)
{
    usleep(time_to_sleep);
    if (arg->num > 100)
    {
        printf("exit in %d\n", arg->index);
        exit(0);
    }
}

int main(void)
{
    philo *var;
    int i = 0;
    pthread_mutex_t i_lock;
    ////////////////////////////////////////////////
    if (pthread_mutex_init(&i_lock, NULL) != 0)
    {
        printf("Error in creating mutex\n");
        exit(0);
    }
    var = malloc(number_of_philosophers * sizeof(philo));
    while (i < number_of_philosophers)
    {
        pthread_mutex_lock(&i_lock);
        var[i] = new_philo(i);
        // usleep()
        i++;
        pthread_mutex_unlock(&i_lock);
    }
    i = 0;

    while (i < number_of_philosophers)
    {
        pthread_mutex_lock(&i_lock);
        if (pthread_create(&var[i].thread, NULL, eat, &var[i]) != 0)
        {
            printf("Error in creating thread\n");
            exit(0);
        }
        usleep(100);
        pthread_mutex_unlock(&i_lock);
        // if (pthread_mutex_init(&var[i].lock, NULL) != 0)
        // {
        //     printf("Error in creating mutex\n");
        //     exit(0);
        // }
        i++;
    }
    // exit(0);
    //////////////////////////////////////////////
    while (1)
    {
        pthread_mutex_lock(&i_lock);
        i = 0;
        while (i < number_of_philosophers)
        {
            usleep(time_to_sleep);
            pthread_mutex_lock(&var[i].lock);
            check(&var[i]);
            pthread_mutex_unlock(&var[i].lock);
            i++;
        }
        pthread_mutex_unlock(&i_lock);
    }
}