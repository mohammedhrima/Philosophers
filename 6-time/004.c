#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
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
    return (now->tv_sec * MILLION + now->tv_usec);
}

#if 0
void my_sleep(t_micro_sec sleeping_timing) // take argument in milisecond
{
    t_micro_sec end;
    t_micro_sec start;

    start = current_time();
    while (1)
    {
        if (current_time() >= start + sleeping_timing * THOUSAND)
            break;
    }
}
#else
void my_sleep(t_micro_sec sleeping_timing) // take argument in milisecond
{
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    while (1)
    {
        usleep(50);
        gettimeofday(&end, NULL);
        if (end.tv_sec * MILLION + end.tv_usec >= start.tv_sec * MILLION + start.tv_usec + sleeping_timing * THOUSAND)
            break;
    }
}
#endif

int main(void)
{
    useconds_t original_timing_to_sleep = 3000;

    // useconds_t real_timing_to_sleep = original_timing_to_sleep - 10000;
    useconds_t recovering = 0;

    struct timeval now0;
    struct timeval now1;
    struct timeval now2;
    struct timeval now3;

    gettimeofday(&now0, NULL);
    my_sleep(original_timing_to_sleep);
    gettimeofday(&now2, NULL);

    printf("now0: %lu sec and %d microsec -> %lu\n", now0.tv_sec, now0.tv_usec, (now0.tv_sec * MILLION + (long)now0.tv_usec));
    // printf("now1: %lu sec and %d microsec -> %lu\n", now1.tv_sec, now1.tv_usec, time1);
    printf("now2: %lu sec and %d microsec -> %lu\n", now2.tv_sec, now2.tv_usec, (now2.tv_sec * MILLION + (long)now2.tv_usec));

    printf("did take %lu\n", (now2.tv_sec * MILLION + (long)now2.tv_usec) - (now0.tv_sec * MILLION + (long)now0.tv_usec));
}