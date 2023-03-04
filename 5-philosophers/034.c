#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

// consts
#define THOUSAND 1000
#define MILLION 1000000
typedef unsigned long t_micro_sec;

t_micro_sec current_time(void)
{
    struct timeval *now;

    now = malloc(sizeof(struct timeval));
    gettimeofday(now, NULL);
    // tv_sec: seconds
    // tv_usec: micro_seconds
    return (now->tv_sec * MILLION + now->tv_usec);
}

void my_sleep(t_micro_sec sleeping_timing) // takes argument in milisecond
{
    t_micro_sec end;
    t_micro_sec curr;

    curr = current_time();
    end = curr + sleeping_timing * THOUSAND; // in micro seconds
    while (curr < end)
        curr = current_time();
}

int main(void)
{
    printf("-> %lu\n", current_time());
    my_sleep(3000);
    printf("--> %lu\n", current_time());
}