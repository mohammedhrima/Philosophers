#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define FAILED != 0

char message[] = "Hello world\n";

void *my_function(void *arg)
{
    printf("function is running..., Argument is %s\n", (char *)arg);
    sleep(3);
    strcpy(message, "Bye!");
    pthread_exit("Thread  exit");
}

int main(void)
{
    pthread_t thread_id;
    int res;
    void *thread_res;

    // create new threads
    // execute my_fuunction in this new thread
    res = pthread_create(&thread_id, NULL, my_function, (void *)message);
    if (res FAILED)
    {
        perror("Error in creating thread\n");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for thread to finish...\n");
    res = pthread_join(thread_id, &thread_res);
    if (res FAILED)
    {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined, it returns %s\n",(char*)thread_res);
    printf("Message is now %s\n", message);
    exit(EXIT_SUCCESS);
}