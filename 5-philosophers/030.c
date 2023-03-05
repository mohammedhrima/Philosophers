#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define thousand 1000
#define million 1000000

useconds_t time_to_sleep = 200000;
useconds_t time_to_eat = 200000;
time_t time_to_die = 800000;
// useconds_t time_to_think = 100000;
// useconds_t usleep_timing = 50000;
useconds_t usleep_timing = 0;
int number_of_philos = 5;

// struct
typedef struct timeval timing;
typedef struct s_philo
{
    int index;
    pthread_t thread;
    time_t last_time_did_eat;
    pthread_mutex_t fork_mutex;
    pthread_mutex_t last_time_did_eat_mutex;
    pthread_mutex_t *printing_mutex;
    struct s_philo *next;
} t_philo;

t_philo *new_philo(int index)
{
    t_philo *res = malloc(sizeof(t_philo));
    res->index = index + 1;
    res->next = NULL;
    pthread_mutex_init(&res->fork_mutex, NULL);
    pthread_mutex_init(&res->last_time_did_eat_mutex, NULL);
    printf("new philo %d\n", res->index);
    return (res);
}

t_philo *init_philos(int len)
{
    int i = 1;
    t_philo *curr = new_philo(0);
    t_philo *head = curr;
    while (i < len)
    {
        curr->next = new_philo(i);
        curr = curr->next;
        i++;
    }
    curr->next = head;
    return (head);
}

void print_state(t_philo *philo, char *str)
{
    pthread_mutex_lock(philo->printing_mutex);
    printf("%d %s\n", philo->index, str);
    pthread_mutex_unlock(philo->printing_mutex);
}

void *routine(void *arg)
{
    while (1)
    {
        t_philo *philo = (t_philo *)arg;
        timing current_time;
        if (pthread_mutex_lock(&philo->fork_mutex) == 0 && pthread_mutex_lock(&philo->next->fork_mutex) == 0)
        {
            gettimeofday(&current_time, NULL);
            pthread_mutex_lock(&philo->last_time_did_eat_mutex);
            philo->last_time_did_eat = current_time.tv_sec /*+ current_time.tv_usec*/ + time_to_eat;
            pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
            /////////////////////////////////////////////////////////
            // take the fork
            // print_state(philo, "has taken fork");
            // print_state(philo, "has taken fork");
            pthread_mutex_lock(philo->printing_mutex);
            printf("%d has taken fork %d \n", philo->index, philo->index);
            printf("%d has taken fork %d \n", philo->index, philo->next->index);
            pthread_mutex_unlock(philo->printing_mutex);
            print_state(philo, "is eating");
            usleep(time_to_eat);
        }
        pthread_mutex_unlock(&philo->fork_mutex);
        pthread_mutex_unlock(&philo->next->fork_mutex);
        print_state(philo, "is sleeping");
        usleep(time_to_sleep);
        print_state(philo, "is thinking");
    }
    return (NULL);
}

// add a usleep here
void check(t_philo *philo)
{
    // t_philo *curr = var;
    timing current_time;
    while (1)
    {
#if 1
        // usleep(usleep_timing);
        // add lock here
        gettimeofday(&current_time, NULL);
        pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        if (current_time.tv_sec + current_time.tv_usec - philo->last_time_did_eat >= time_to_die)
        {
            printf("%d did died\n", philo->index);
            exit(0);
        }
        pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        philo = philo->next;
#endif
    }
}

int main(void)
{
    int len = number_of_philos;
    t_philo *philo = init_philos(len);
    pthread_mutex_t *printing_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(printing_mutex, NULL);

    printf("\n");
    int i = 0;
    while (i < len)
    {
        philo->printing_mutex = printing_mutex;
        philo = philo->next;
        i++;
    }
    printf("\n");
    // init timing
    timing current_time;
    gettimeofday(&current_time, NULL);

    i = 0;
    while (i < len)
    {
        if (i % 2 == 0)
        {
            philo->last_time_did_eat = current_time.tv_sec + current_time.tv_usec;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    i = 0;
    while (i < len)
    {
        if (i % 2 == 1)
        {
            philo->last_time_did_eat = current_time.tv_sec + current_time.tv_usec;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    check(philo);
}