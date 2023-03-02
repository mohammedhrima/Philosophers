#include <pthread.h>

typedef struct 
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
} var;

int main(void)
{
    var *data;
    int status;
    status = pthread_mutex_init(&data->mutex, NULL);
    status = pthread_cond_init(&data->cond, NULL);

    pthread_mutex_destroy(&data->mutex);
}
