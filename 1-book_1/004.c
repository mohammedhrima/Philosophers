#include <pthread.h>
#include <time.h>

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
} var;

// create condition (mutex) statically
var data = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    0};

// create

int main(void)
{
    return (0);
}