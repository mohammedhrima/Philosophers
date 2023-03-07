#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int time_to_sleep = 300000;

typedef struct my_var
{
    int index;
    int c;
    pthread_mutex_t *lock;
    pthread_t thread;
} my_var;

void *routine(void *arg)
{
    while (1)
    {
        my_var *var = (my_var *)arg;
        usleep(time_to_sleep);
        pthread_mutex_lock(var->lock);
        var->c++;
        printf("increment in var[%d] c=%d\n", var->index, var->c);
        if(var->c == 5)
        {
            usleep(time_to_sleep);
            printf("exit in var %d\n", var->index);
            exit(0);
        }
        usleep(time_to_sleep);
        pthread_mutex_unlock(var->lock);
    }
    return (NULL);
}

// int check(my_var *var)
// {
//     if (var->c == 5)
//     {
//         printf("exit in var %d\n", var->index);
//         exit(0);
//     }
//     return 0;
// }

my_var *new_var(int index)
{
    my_var *res = malloc(sizeof(my_var));
    res->c = 0;
    res->index = index;
    res->lock = NULL;
    // res->lock = malloc(sizeof(pthread_t));
    printf("new var %d is created\n", res->index);
    return (res);
}
int len = 10;
int main(void)
{
    my_var *var[len];
    pthread_mutex_t i_lock;

    // common lock
    pthread_mutex_t *lock;
    lock = malloc(sizeof(pthread_mutex_t));

    pthread_mutex_init(lock, NULL);
    int i = 0;
    while (i < len)
    {
        // printf("59\n");
        var[i] = new_var(i);
        var[i]->lock = lock;
        i++;
    }

    i = 0;
    while (i < len)
    {
        if (pthread_create(&(var[i]->thread), NULL, routine, var[i]) != 0)
        {
            printf("Error in creating thread %d\n", var[i]->index);
            exit(-1);
        }
        i++;
    }

    i = 0;
    while (1)
    {
        // i = 0;
        // while (i < len)
        // {
        //     pthread_mutex_lock(lock);
        //     check(var[i]);
        //     i++;
        //     pthread_mutex_unlock(lock);
        // }
    }
}