#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int time_to_sleep = 300000;

typedef struct my_var
{
    int index;
    int i;
    pthread_t thread;
    pthread_mutex_t lock_index;
} my_var;

void *eat(void *arg)
{
    while (1)
    {
        usleep(time_to_sleep);
        my_var *var = (my_var *)arg;
        // pthread_mutex_lock(&var->lock_index);
        printf("increment var[%d].i=%d\n", var->index, var->i);
        var->i++;
        // pthread_mutex_unlock(&var->lock_index);
    }
    return (NULL);
}

void check(int i)
{
    usleep(time_to_sleep);
    if (i > 100)
    {
        printf("exit\n");
        exit(0);
    }
}

my_var *new_var(int i)
{
    my_var *res = malloc(sizeof(my_var));
    res->i = i;
    res->index = i;
    if (pthread_create(&res->thread, NULL, eat, res) != 0)
    {
        printf("Error in creating thread %d\n", i);
        exit(-1);
    }
    if (pthread_mutex_init(&res->lock_index, NULL) != 0)
    {
        printf("Error in creating mutex %d\n", i);
    }
    return (res);
}
#define len 3
int main(void)
{
    my_var *var[len];
    pthread_mutex_t i_lock;
    int i = 0;
    while (i < len)
    {
        // printf("line 55 -> %d\n", i);
        var[i] = new_var(i);
        i++;
    }
    i = 0;
    // printf("line 56\n");
    while (1)
    {
        // pthread_mutex_lock(&i_lock);
        // pthread_mutex_lock(&var[i]->lock_index);
        // usleep(3000);
        printf("check from %d\n", var[i]->index);
        check(var[i]->i);
        // pthread_mutex_unlock(&var[i]->lock_index);
        i++;
        if (i == len)
            i = 0;
        // pthread_mutex_unlock(&i_lock);
    }
}