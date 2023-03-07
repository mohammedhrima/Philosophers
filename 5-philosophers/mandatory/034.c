#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// consts
#define THOUSAND 1000
#define MILLION 1000000

// structs
typedef struct timeval t_timing;
typedef pthread_mutex_t t_mutex;
typedef pthread_t t_thread;


typedef struct s_shared
{
    time_t starting_time;

    // giving inputs
    time_t time_to_eat;
    time_t time_to_sleep;
    time_t time_to_die;
    
    int number_of_philos;
    int number_of_meals;

    t_mutex printing_mutex;
    t_mutex checking_mutex;

} t_shared;

typedef struct s_philo
{
    int index;

    time_t last_time_did_eat;
    t_thread thread;

    t_mutex fork_mutex;
    t_mutex last_time_did_eat_mutex;

    t_shared *data;

    struct s_philo *next;
} t_philo;

void my_sleep(time_t sleeping_timing) // take argument in miliseconds
{
#if 0
    usleep(sleeping_timing * THOUSAND);
#elif 1
    struct timeval start;
    struct timeval end;
    gettimeofday(&start, NULL);
#if 0
    printf("\ngaping will be %u in each rotation\n", gap_timing);
    printf("%lu start\n%lu end\n", start.tv_sec * MILLION + start.tv_usec, start.tv_sec * MILLION + start.tv_usec + sleeping_timing);
#endif
    while (1)
    {
        usleep(5);
        gettimeofday(&end, NULL);
        // if (end.tv_sec * MILLION + end.tv_usec >= start.tv_sec * MILLION + start.tv_usec + sleeping_timing * THOUSAND)
        //     break;
        if (end.tv_sec * THOUSAND + end.tv_usec / THOUSAND >= start.tv_sec * THOUSAND + start.tv_usec / THOUSAND + sleeping_timing)
            break;
    }
#endif
}

t_philo *new_philo(int index, t_shared *data)
{
    t_philo *philo;

    philo = malloc(sizeof(t_philo));
    philo->index = index + 1;

    // init mutexes
    pthread_mutex_init(&philo->fork_mutex, NULL);
    pthread_mutex_init(&philo->last_time_did_eat_mutex, NULL);

    // philo->number_of_meals_eaten = 0;
    philo->next = NULL;

    philo->data = data;

    return (philo);
}

t_shared *new_shared(time_t time_to_eat, time_t time_to_die, time_t time_to_sleep, int number_of_meals)
{
    t_shared *data;

    data = malloc(sizeof(t_shared));

    // init timings, convert miliseconds to micro
    data->time_to_eat = time_to_eat;
    data->time_to_sleep = time_to_sleep;
    data->time_to_die = time_to_die;
    data->number_of_meals = number_of_meals;

    // init mutexes
    pthread_mutex_init(&data->printing_mutex, NULL);
    pthread_mutex_init(&data->checking_mutex, NULL);


    return (data);
}

t_philo *init_philos(int number_of_philos, t_shared *data)
{
    int i = 1;
    t_philo *curr = new_philo(0, data);
    curr->next = curr;
    t_philo *head = curr;
    while (i < number_of_philos)
    {
        curr->next = new_philo(i, data);
        curr = curr->next;
        i++;
    }
    curr->next = head;
    return (head);
}

void take_fork(t_philo *philo, t_mutex *token_fork, int fork_index)
{
    t_timing current_time;

    pthread_mutex_lock(token_fork);
    // get time
    gettimeofday(&current_time, NULL);
    /////////////////////////////////////////////////////////////////////////////////////
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d has taken fork %d\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index, fork_index);
    pthread_mutex_unlock(&philo->data->printing_mutex);
}

void eating(t_philo *philo)
{
    t_timing current_time;

    // get time
    gettimeofday(&current_time, NULL);

    // check last time did eat
    pthread_mutex_lock(&philo->last_time_did_eat_mutex);
    philo->last_time_did_eat = current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time;
    // philo->number_of_meals_eaten++;
    pthread_mutex_unlock(&philo->last_time_did_eat_mutex);

    // print state
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d is eating\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
    pthread_mutex_unlock(&philo->data->printing_mutex);

    // sleep eating time
    my_sleep(philo->data->time_to_eat);

    // unlock forks
    pthread_mutex_unlock(&philo->fork_mutex);
    pthread_mutex_unlock(&philo->next->fork_mutex);
}

void sleeping(t_philo *philo)
{
    t_timing current_time;

    // get time
    gettimeofday(&current_time, NULL);

    // print state
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d is sleeping\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
    pthread_mutex_unlock(&philo->data->printing_mutex);

    // sleep
    my_sleep(philo->data->time_to_sleep);
}

void thinking(t_philo *philo)
{
    t_timing current_time;

    // get time
    gettimeofday(&current_time, NULL);

    // print state
    pthread_mutex_lock(&philo->data->printing_mutex);
    printf("%10lu: %d is thinking\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
    pthread_mutex_unlock(&philo->data->printing_mutex);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_timing current_time;

    while (1)
    {
        // lock for checking ?
        // if (philo->number_of_meals_eaten < philo->data->number_of_meals)
        // {
            // take first fork
            take_fork(philo, &philo->fork_mutex, philo->index);

            // take second fork
            take_fork(philo, &philo->next->fork_mutex, philo->next->index);

            // start eating
            eating(philo);
        // }

        // start sleeping
        sleeping(philo);

        // start thinking
        thinking(philo);
    }
    return (NULL);
}

// void exit_if_all_philos_finished_eating(t_philo *philo)
// {
//     int i = 0;
//     while (i < philo->data->number_of_philos)
//     {
//         if (philo->number_of_meals_eaten < philo->data->number_of_meals)
//             return;
//         philo = philo->next;
//         i++;
//     }
//     pthread_mutex_lock(&philo->data->printing_mutex);
//     printf("line 245 philo %d\n", philo->index);
//     exit(0);
// }

void check(t_philo *philo)
{
    time_t time_to_die = philo->data->time_to_die;
    t_timing current_time;
    int i = 0;
    while (1)
    {
        my_sleep(5);
        gettimeofday(&current_time, NULL);
        pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        pthread_mutex_lock(&philo->data->printing_mutex);
        if (current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time - philo->last_time_did_eat >= time_to_die)
        {
            printf("%10lu: %d did died\n", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);
            exit(0);
        }
        pthread_mutex_unlock(&philo->data->printing_mutex);
        pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        // exit_if_all_philos_finished_eating(philo);
        philo = philo->next;
    }
}

// don't forget duk ltest dyul walu in atoi ...
// check pthread functions and gettimeofday if they failed
int main(void)
{
    int i;
    t_timing current_time;

    // giving variables from input
    // convert them from miliseconds to micro seconds
    // handle 1 philosopher
    //
    /*
  1 800 200 200 / not eat and die
  5 800 200 200 / no one die
  4 410 200 200 / no one die
  4 310 200 100 / one should die
  2

   lbehja :
    int number_of_philos = 150;
    time_t time_to_die = 60;
    time_t time_to_eat = 60;
    time_t time_to_sleep = 60;
    int number_of_meals = 5;
  */
    int number_of_philos = 4;
    time_t time_to_die = 410;
    time_t time_to_eat = 200;
    time_t time_to_sleep = 200;
    int number_of_meals = 5;

    // ft_putnbr(time_to_die);
    // exit(0);

    // init shared data
    t_shared *data = new_shared(time_to_eat, time_to_die, time_to_sleep, number_of_meals);
    // init philos
    t_philo *philo = init_philos(number_of_philos, data);

    // create threads
    gettimeofday(&current_time, NULL);
    data->starting_time = current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND;
    // data->starting_time = current_time.tv_sec * THOUSAND;

    i = 0;
    while (i < number_of_philos)
    {
        if (i % 2 == 0)
        {
            philo->last_time_did_eat = 0;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    my_sleep(10);
    i = 0;
    while (i < number_of_philos)
    {
        if (i % 2 == 1)
        {
            philo->last_time_did_eat = 0;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    // add check here
    check(philo);
}
