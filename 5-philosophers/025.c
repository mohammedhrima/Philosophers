#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

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
    // struct timeval current;
    // gettimeofday(&current, NULL);
    // printf("seconds: %ld micro seconds: %d\n", current.tv_sec, current.tv_usec);
    while (1)
    {
        time_spent();
        usleep(300000);
    }

    return (0);
}