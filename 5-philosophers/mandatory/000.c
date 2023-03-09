#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>


typedef struct 
{
    pthread_mutex_t *lock;
    pthread_t thread;
} var;


int n = 0;

void *func1(void *arg)
{
    while (1)
    {
        usleep(60000);
        pthread_mutex_lock(((var*)arg)->lock);
        n++;
        printf("call from func1 -> %d\n",n);
        // pthread_mutex_unlock(((var*)arg)->lock);
    }
    
}

void *func2(void *arg)
{
    while (1)
    {
        usleep(60000);
        // pthread_mutex_lock(((var*)arg)->lock);
        n--;
        printf("call from func2 -> %d\n",n);
        pthread_mutex_unlock(((var*)arg)->lock);
    }
    
}

int main(void)
{
    var first;
    var second;

    pthread_mutex_t lock;
    // pthread_t thread1;
    // pthread_t thread2;
    pthread_mutex_init(&lock, NULL);
    first.lock = &lock;
    second.lock = &lock;

    pthread_create(&first.thread,NULL, func1, &first );
    pthread_create(&second.thread,NULL, func2, &second );
    while(1);
}