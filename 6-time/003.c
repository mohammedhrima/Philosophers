#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

#define MILLION 1000000
#define THOUSAND 1000


int main(void)
{
    useconds_t original_timing_to_sleep = 300000;

    useconds_t real_timing_to_sleep = original_timing_to_sleep - 10000;
    useconds_t recovering = 0;

    struct timeval now0;
    struct timeval now1;
    struct timeval now2;
    struct timeval now3;

    // gettimeofday(&now0, NULL);
    // usleep(real_timing_to_sleep);
    // // gettimeofday(&now1, NULL);
    // // usleep(10000 - (now1.tv_usec - now0.tv_usec));
    // gettimeofday(&now2, NULL);

    // long time0 = now0.tv_sec * MILLION + (long)now0.tv_usec;
    // // long time1 = now1.tv_sec * MILLION + (long)now1.tv_usec;
    // long time2 = now2.tv_sec * MILLION + (long)now2.tv_usec;
    // // recover
    // usleep(original_timing_to_sleep - (time2 - time0));
    
    gettimeofday(&now0, NULL);
    // long time3 = now3.tv_sec * MILLION + (long)now3.tv_usec;


    printf("now: %lu sec and %d\n", now0.tv_sec, now0.tv_usec);
    printf("now: %lu \n", now0.tv_sec* MILLION + now0.tv_usec);
    printf("now: %lu \n", (now0.tv_sec* MILLION + now0.tv_usec) / THOUSAND);


    // printf("now1: %lu sec and %d microsec -> %lu\n", now1.tv_sec, now1.tv_usec, time1);
    // printf("now2: %lu sec and %d microsec -> %lu\n", now2.tv_sec, now2.tv_usec, time2);

    // printf("did take %lu\n", time3 - time0);
}