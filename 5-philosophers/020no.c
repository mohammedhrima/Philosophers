#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int time_to_sleep = 300000;
pthread_mutex_t check_lock;

typedef struct my_var
{
    int index;
    int i;
    int left;
    int right;
    pthread_t thread[5];
    pthread_mutex_t lock[5];
} my_var;

typedef struct array
{
    my_var var[5];
    int len;
} array;

void check(my_var *var, int i)
{
    printf("27\n");
    // pthread_mutex_lock(&check_lock);
    if (var[i].i >= 5)
    {
        printf("exit in %d\n", var->index);
        exit(0);
    }
    // pthread_mutex_unlock(&check_lock);
}
void *routine(void *arg)
{
    my_var *var = (my_var *)arg;
    while (1)
    {
        printf("41\n");
        // usleep(time_to_sleep);
        // pthread_mutex_lock(&check_lock);
        // pthread_mutex_lock(&var->lock);
        pthread_mutex_lock(&var->lock[var->left]);
        pthread_mutex_lock(&var->lock[var->right]);

        printf("->%d\n", var->index);
        var->i++;
        pthread_mutex_unlock(&var->lock[var->right]);
        pthread_mutex_unlock(&var->lock[var->left]);
        // pthread_mutex_unlock(&check_lock);
    }
    return (NULL);
}

my_var new_philo(int index, int len)
{
    my_var new;

    new.index = index;
    new.left = index;
    new.right = index + 1;
    if (new.right == len)
        new.right = 0;
    new.i = 0;
    // pthread_create(&new.thread, NULL, routine, &new);
    // pthread_mutex_init(&new.lock, NULL);
    return (new);
}

int main(void)
{
    int len = 5;
    int i = 0;
    // array arr;

    my_var var;
    my_var philos[len];
    // pthread_mutex_t locks[len];
    pthread_mutex_init(&check_lock, NULL);

    while (i < len)
    {
        pthread_mutex_init(&var.lock[i], NULL);
        i++;
    }
    // initialisation philos
    i = 0;
    while (i < len)
    {
        philos[i] = new_philo(i, len);
        i++;
    }
    /// threads creation
    i = 0;
    while (i < len)
    {
        pthread_create(&var.thread[i], NULL, routine, &var);
        i++;
        usleep(500);
    }
    // checker
    while (1)
    {
        // i = 0;
        // while (i < len)
        // {
        //     printf("109\n");
        //     check(philos, i);
        //     i++;
        // }
    }
}