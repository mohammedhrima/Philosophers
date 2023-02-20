#include <stdio.h>
#include <pthread.h>

void *func1(void *thread_ptr)
{
    int n = *((int*)thread_ptr);
    printf("run func from %d\n",n);
    *((int*)thread_ptr) = 20;
    return (NULL);
}

int main(void)
{
    pthread_t thread_id;
    int j = 10;

    pthread_create(&thread_id, NULL, func1, &j);

    // wait for func1
    int ptr;
    pthread_join(thread_id, (void *)&ptr);

    printf("int main j=%d\n", j);
    return(0);
}