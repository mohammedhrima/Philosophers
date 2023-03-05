#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define million 1000000
#define sec tv_sec
#define milsec tv_usec
typedef struct timeval timing;
void time_spent(void)
{
    static timing *start;
    timing *current;

    if (!start)
    {
        start = malloc(sizeof(timing));
        gettimeofday(start, NULL);
    }
    current = malloc(sizeof(timing));
    gettimeofday(current, NULL);

    printf("you did spent %lu sec\n", current->tv_sec - start->tv_sec);
}

int main(void)
{
    struct timeval current;
    gettimeofday(&current, NULL);
    // printf("%ld seconds, %d | %ld micro seconds\n", current.tv_sec, current.tv_usec, (time_t)current.tv_usec);
    printf("%ld seconds + %ld micro seconds = %ld\n", current.tv_sec, (time_t)current.tv_usec, million * current.tv_sec + (time_t)current.tv_usec);
    printf("%ld seconds + %ld micro seconds = %ld\n", current.sec, (time_t)current.milsec, million * current.sec + (time_t)current.milsec);


    // while (1)
    // {
    //     time_spent();
    //     usleep(300000);
    // }

    return (0);
}
