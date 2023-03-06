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

// to be removed after, sleep between threads in miliseconds
time_t sleep_between_threads = 0.5 * THOUSAND;

typedef struct s_shared
{
    time_t time_to_print;
    time_t starting_time;
    // giving inputs
    time_t time_to_eat;
    time_t time_to_sleep;
    time_t time_to_die;
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

    time_t last_time_did_eat;
    t_thread thread;

    t_mutex fork_mutex;
    t_mutex last_time_did_eat_mutex;
    t_mutex number_of_meals_eaten_mutex;

    int number_of_meals_eaten;
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
    useconds_t gap_timing;
    useconds_t total_gap_timing;

    // calculate how much time i should sleep
    // every time to gap  0 - 0.2 miliseconds in total
    gettimeofday(&start, NULL);
#if 0
    printf("\ngaping will be %u in each rotation\n", gap_timing);
    printf("%lu start\n%lu end\n", start.tv_sec * MILLION + start.tv_usec, start.tv_sec * MILLION + start.tv_usec + sleeping_timing);
#endif
    while (1)
    {
        usleep(50); // take time in micro seconds
        gettimeofday(&end, NULL);
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
    pthread_mutex_init(&philo->number_of_meals_eaten_mutex, NULL);


    philo->number_of_meals_eaten = 0;
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

    // meals
    data->number_of_meals = number_of_meals;

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

void print_state(t_philo *philo, char *str, time_t current_time_in_miliseconds, int fork_index)
{
    pthread_mutex_lock(&philo->data->printing_mutex);
    if (fork_index)
        printf("%10lu: %d %s %d\n", current_time_in_miliseconds, philo->index, str, fork_index);
    else
    {
        if (philo)
            printf("%10lu: %d %s\n", current_time_in_miliseconds, philo->index, str);
        else
            printf("%10lu: %s\n", current_time_in_miliseconds, str);
    }
    printf("philos meals now %d\n" , philo->number_of_meals_eaten);
    pthread_mutex_unlock(&philo->data->printing_mutex);
}

void *routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_timing current_time;
    // t_timing current_time1;
    // t_timing current_time2;

    while (1)
    {
        // lock for checking ?
        // take first fork

        // take first fork
        pthread_mutex_lock(&philo->fork_mutex);
        gettimeofday(&current_time, NULL);
        print_state(philo, "has taken a fork", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->index);

        // take second fork
        pthread_mutex_lock(&philo->next->fork_mutex);
        gettimeofday(&current_time, NULL);
        print_state(philo, "has taken a fork", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, philo->next->index);

        // start eating
        pthread_mutex_lock(&philo->last_time_did_eat_mutex);
        gettimeofday(&current_time, NULL);
        philo->last_time_did_eat = current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time + philo->data->time_to_eat;
        pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
        // mutex for number of meals
        print_state(philo, "is eating", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, 0);
        my_sleep(philo->data->time_to_eat);
        pthread_mutex_lock(&philo->number_of_meals_eaten_mutex);
        philo->number_of_meals_eaten++;
        pthread_mutex_unlock(&philo->number_of_meals_eaten_mutex);

        // unlock forks after eating
        pthread_mutex_unlock(&philo->next->fork_mutex);
        pthread_mutex_unlock(&philo->fork_mutex);

        // start sleeping
        gettimeofday(&current_time, NULL);
        print_state(philo, "is sleeping", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, 0);
        my_sleep(philo->data->time_to_sleep);

        // start thinking
        gettimeofday(&current_time, NULL);
        print_state(philo, "is thinking", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, 0);
    }
    return (NULL);
}

void check(t_philo *philo)
{
    time_t time_to_die = philo->data->time_to_die;
    t_timing current_time;
    int all_philos_finished_number_fo_meals;
    int i = 0;
    while (1)
    {
        usleep(50);
        i = 0;
        all_philos_finished_number_fo_meals = 1;
        while (i < philo->data->number_of_philos)
        {
            pthread_mutex_lock(&philo->last_time_did_eat_mutex);
            gettimeofday(&current_time, NULL);
            if (current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time - philo->last_time_did_eat >= time_to_die)
            {
                // printf("last time did finish eating %lu, and current time %lu\n", philo->last_time_did_eat, current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time);
                print_state(philo, "did died", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, 0);
                exit(0);
            }
            pthread_mutex_unlock(&philo->last_time_did_eat_mutex);
            pthread_mutex_lock(&philo->number_of_meals_eaten_mutex);
            if (philo->number_of_meals_eaten < philo->data->number_of_meals)
                all_philos_finished_number_fo_meals = 0;
            pthread_mutex_unlock(&philo->number_of_meals_eaten_mutex);
            philo = philo->next;
            i++;
        }
        gettimeofday(&current_time, NULL);
        if (all_philos_finished_number_fo_meals)
        {
            print_state(NULL, "all philos finished there meals", current_time.tv_sec * THOUSAND + current_time.tv_usec / THOUSAND - philo->data->starting_time, 0);
            exit(0);
        }
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
    /*
    1 800 200 200 / not eat and die
    5 800 200 200 / no one die
    4 410 200 200 / no one die
    4 310 200 100 / one should die
    2
    */
    int number_of_philos = 3;
    time_t time_to_die = 401;
    time_t time_to_eat = 200;
    time_t time_to_sleep = 200;
    int number_of_meals = 5;

    // ft_putnbr(time_to_die);
    // exit(0);

    // init shared data
    t_shared *data = new_shared(time_to_eat, time_to_die, time_to_sleep, number_of_meals);
    // init philos
    t_philo *philo = init_philos(number_of_philos, data);

    // i = 0;
    // while (i < number_of_philos)
    // {
    //     printf("philo index %d, next index %d\n", philo->index, philo->next->index);
    //     philo = philo->next;
    //     i++;
    // }

    // exit(0);
    // printf("\n\n");

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
