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
typedef unsigned long t_micro_sec;

// to be removed after
t_micro_sec sleep_between_threads = 1000 * THOUSAND;

typedef struct s_shared
{
    t_micro_sec time_to_print;
    // giving inputs
    t_micro_sec time_to_eat;
    t_micro_sec time_to_die;
    t_micro_sec time_to_sleep;
    int number_of_philos;
    int number_of_meals;

    // conditions for exiting
    int all_alive;
    int all_philos_did_eat_number_of_meals;

    t_mutex printing_mutex;
    t_mutex checking_mutex;

} t_shared;

typedef struct s_philo
{
    int index;

    t_micro_sec last_time_did_eat;
    t_thread thread;

    t_mutex fork_mutex;
    t_mutex last_time_did_eat_mutex;

    int did_finish_all_meals;
    t_shared *data;

    struct s_philo *next;
} t_philo;

void my_sleep(t_micro_sec sleeping_timing) // take argument in microseconds
{
    struct timeval start;
    struct timeval end;
    useconds_t scaling;

    // calculate how much time i should sleep
    // every time to gap  0 - 0.2 miliseconds in total
    scaling = (20 * MILLION) / sleeping_timing;
    gettimeofday(&start, NULL);
    printf("scaling will be %u \n", scaling);
    printf("%lu start\n%lu end\n\n", start.tv_sec * MILLION + start.tv_usec, start.tv_sec * MILLION + start.tv_usec + sleeping_timing);

    // exit(0);
    while (1)
    {
        usleep(scaling); // take time in micro seconds
        // usleep(sleeping_timing / 2);
        gettimeofday(&end, NULL);
        if (end.tv_sec * MILLION + end.tv_usec >= start.tv_sec * MILLION + start.tv_usec + sleeping_timing)
            break;
        // exit(0);
    }
    printf("%lu now\n", end.tv_sec * MILLION + end.tv_usec);
}

t_philo *new_philo(int index, t_shared *data)
{
    t_philo *philo;

    philo = malloc(sizeof(t_philo));
    philo->index = index + 1;

    // init mutexes
    pthread_mutex_init(&philo->fork_mutex, NULL);
    pthread_mutex_init(&philo->last_time_did_eat_mutex, NULL);

    philo->did_finish_all_meals = 0;
    philo->next = NULL;

    philo->data = data;

    printf("new philo %d\n", philo->index);

    return (philo);
}

t_shared *new_shared(t_micro_sec time_to_eat, t_micro_sec time_to_die, t_micro_sec time_to_sleep)
{
    t_shared *data;

    data = malloc(sizeof(t_shared));

    // init timings
    data->time_to_eat = time_to_eat;
    data->time_to_die = time_to_die;
    data->time_to_sleep = time_to_sleep;

    // init mutexes
    pthread_mutex_init(&data->printing_mutex, NULL);
    pthread_mutex_init(&data->checking_mutex, NULL);

    // init exit conditions
    data->all_alive = 1;
    data->all_philos_did_eat_number_of_meals = 0;

    return (data);
}

t_philo *init_philos(int number_of_philos, t_shared *data)
{
    int i = 1;
    t_philo *curr = new_philo(0, data);
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

int ft_strcmp(char *string1, char *string2)
{
    int i;

    i = 0;
    while (string1 && string2 && string1[i] && string1[i] == string2[i])
        i++;
    return (string1[i] - string2[i]);
}

void print_state(t_philo *philo, char *str)
{
    t_timing current_time;

    pthread_mutex_lock(&philo->data->printing_mutex);
    gettimeofday(&current_time, NULL);
    printf("%lu: %d %s", current_time.tv_sec * MILLION + current_time.tv_usec, philo->index, str);
    if (ft_strcmp(str, "has taken fork") == 0)
        printf(" %d %d\n", philo->index, philo->next->index);
    pthread_mutex_unlock(&philo->data->printing_mutex);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_timing current_time;
    while (1)
    {
        my_sleep(sleep_between_threads);
        // lock for checking ?
        pthread_mutex_lock(&philo->fork_mutex);
        pthread_mutex_lock(&philo->next->fork_mutex);

        // take a fork
        gettimeofday(&current_time, NULL);
        pthread_mutex_lock(&philo->data->printing_mutex);
        printf("%lu: %d has taken fork", current_time.tv_sec * MILLION + current_time.tv_usec, philo->index);
        pthread_mutex_lock(&philo->data->printing_mutex);
        // pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        gettimeofday(&current_time, NULL);
        philo->last_time_did_eat = current_time.tv_sec * MILLION + current_time.tv_usec + philo->data->time_to_eat;
        // pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        // start eating
        print_state(philo, "is eating\n");
        my_sleep(philo->data->time_to_eat);

        // unlock forks after eating
        pthread_mutex_unlock(&philo->fork_mutex);
        pthread_mutex_unlock(&philo->next->fork_mutex);

        print_state(philo, "is sleeping\n");
        my_sleep(philo->data->time_to_sleep);
        print_state(philo, "is thinking\n");
    }
    return (NULL);
}

void check(t_philo *philo)
{
    t_micro_sec time_to_eat = philo->data->time_to_eat;
    t_timing current_time;
    while (1)
    {
        my_sleep(sleep_between_threads);
        pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        gettimeofday(&current_time, NULL);
        if (current_time.tv_sec * MILLION + current_time.tv_usec - time_to_eat > philo->last_time_did_eat)
        {
            print_state(philo, "did died\n");
            exit(0);
        }
        pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        philo = philo->next;
    }
}

// don't forget duk ltest dyul walu in atoi ...
// check pthread functions and gettimeofday if they failed
int main(void)
{
    int i;
    t_timing current_time;
    // t_micro_sec sleep_timing_test = 10 /*in seconds*/ * MILLION;
    t_micro_sec sleep_timing_test = 70 /*in miliseconds*/ * THOUSAND;
#if 0
    write(1, "\nbefore my_sleep\n", 17);
    my_sleep(sleep_timing_test);
    write(1, "\nafter my_sleep\n", 17);

#elif 1
    // giving variables from input
    // convert them from miliseconds to micro seconds
    t_micro_sec time_to_sleep = 2000 * THOUSAND;
    t_micro_sec time_to_eat = 1000 * THOUSAND;
    t_micro_sec time_to_die = 8000 * THOUSAND;
    // t_micro_sec time_to_print = 1000;
    int number_of_philos = 6;

    // init shared data
    t_shared *data = new_shared(time_to_eat, time_to_die, time_to_sleep);
    // init philos
    t_philo *philo = init_philos(number_of_philos, data);

    printf("\n\n");


    // create threads
    i = 0;
    while (i < number_of_philos)
    {
        printf("line break\n");
        my_sleep(sleep_between_threads);
        if (i % 2 == 0)
        {
            gettimeofday(&current_time, NULL);
            philo->last_time_did_eat = current_time.tv_sec * MILLION + current_time.tv_usec;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    i = 0;
    while (i < number_of_philos)
    {
        my_sleep(sleep_between_threads);
        if (i % 2 == 1)
        {
            gettimeofday(&current_time, NULL);
            philo->last_time_did_eat = current_time.tv_sec * MILLION + current_time.tv_usec;
            pthread_create(&philo->thread, NULL, routine, philo);
        }
        philo = philo->next;
        i++;
    }
    // add check here
    check(philo);
#endif
}
