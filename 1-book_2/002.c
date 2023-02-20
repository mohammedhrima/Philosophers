#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define FAILED != 0

// char message[] = "Hello world\n";
int run_now = 1;

void *my_function(void *arg)
{
    printf("function is running..., Argument is %d\n", *((int *)arg));
    // sleep(3);
    int print_count1 = 0;
    while (print_count1++ < 20)
    {
        if (run_now == 1)
        {
            printf("1\n");
            run_now = 2;
        }
        else
        {
            sleep(1);
        }
    }
    // strcpy(message, "Bye!");
    pthread_exit("\nThread exit");
}

int main(void)
{
    pthread_t thread_id;
    int res;
    void *thread_res;

    // create new threads
    // execute my_function in this new thread
    res = pthread_create(&thread_id, NULL, my_function, (void *)&run_now);
    if (res fsiled)
    {
        perror("Error in creating thread\n");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for thread to finish...\n");
    res = pthread_join(thread_id, &thread_res);
    if (res fsiled)
    {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined, it returns %p\n", (char *)thread_res);
    // printf("Message is now %s\n", message);
    int print_count1 = 0;
    while (print_count1++ < 20)
    {
        if (run_now == 1)
        {
            printf("1\n");
            run_now = 2;
        }
        else
        {
            sleep(1);
        }
    }
    exit(EXIT_SUCCESS);
}