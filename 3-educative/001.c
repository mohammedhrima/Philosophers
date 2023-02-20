#include <pthread.h>
#include <stdio.h>

int i = 2;

void *my_function(void *thread_ptr)
{
    int k = *(int *)thread_ptr;
    printf("Received value: %i\n", k);

    // return reference to globale variable:
    pthread_exit(&i);
}

int main(void)
{
    pthread_t thread_id;

    int j = 1;
    pthread_create(&thread_id, NULL, my_function, &j);

    int *ptr;

    // wait for my_function
    pthread_join(thread_id, (void **)&ptr);
    printf("Value received by parent form child: %i\n", *ptr);
}