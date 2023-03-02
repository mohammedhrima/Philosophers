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

pthread_mutex_t i_lock;
pthread_mutex_t j_lock;


void *check(void *arg)
{
    my_var *var = (my_var *)arg;
    while (1)
    {
        usleep(time_to_sleep);
        pthread_mutex_lock(&j_lock);
        if (var->i >= 5)
        {
            printf("exit in var[%d] \n", var->index);
            exit(0);
        }
        pthread_mutex_unlock(&j_lock);
    }
    return (NULL);
}

void eat(my_var *var)
{
    // while (1)
    // {

    // check((void*)var);
    usleep(time_to_sleep);
    // my_var *var = (my_var *)arg;
    pthread_mutex_lock(&var->lock_index);
    // printf("increment var[%d].i=%d\n", var->index, var->i);
    var->i++;
    printf("var[%d] is eating: %d \n", var->index, var->i);
    pthread_mutex_unlock(&var->lock_index);
    // }
    // return (NULL);
}

my_var *new_var(int i)
{
    my_var *res = malloc(sizeof(my_var));
    res->i = i;
    res->index = i;
    if (pthread_create(&res->thread, NULL, check, res) != 0)
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

    int i = 0;
    while (i < len)
    {
        // printf("line 55 -> %d\n", i);
        var[i] = new_var(i);
        i++;
    }
    i = 0;
    pthread_mutex_init(&i_lock, NULL);
    pthread_mutex_init(&j_lock, NULL);

    // printf("line 56\n");
    while (1)
    {
        pthread_mutex_lock(&i_lock);
        eat(*(var + i));
        pthread_mutex_unlock(&i_lock);

        // check(var[i]->i);
        // pthread_mutex_unlock(&var[i]->lock_index);
        i++;
        if (i == len)
            i = 0;
        // pthread_mutex_unlock(&i_lock);
    }
}