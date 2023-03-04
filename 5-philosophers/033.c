#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

// consts
#define THOUSAND 1000
#define MILLION 1000000
typedef unsigned long t_micro_sec;

t_micro_sec current_time(void)
{
    struct timeval *now;

    now = malloc(sizeof(struct timeval));
    gettimeofday(now, NULL);
    // return (now->tv_sec * MILLION + now->tv_usec);
    return (now->tv_sec * THOUSAND);
}

void my_sleep(t_micro_sec sleeping_timing) // take argument in milisecond
{
    // usleep(sleeping_timing);
    t_micro_sec end;
    t_micro_sec curr;

    curr = current_time();
    end = curr + sleeping_timing;

    printf("start from %lu, sleep %lu, will stop in %lu\n", curr, sleeping_timing, end);
    while (curr < end)
    {
        curr = current_time();
    }
    printf("now %lu | stop in %lu\n", curr, end);
}

int main(void)
{
    printf("-> %lu\n", current_time());
    my_sleep(3000);
    printf("--> %lu\n", current_time());
}