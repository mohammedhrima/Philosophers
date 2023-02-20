#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
// #include "errors.h"

/*
 * Define a structure, with a mutex.
 */
typedef struct my_struct_tag
{
    pthread_mutex_t mutex; /* Protects access to value */
    int value;             /* Access protected by mutex */
} my_struct_t;

int main(int argc, char *argv[])
{
    my_struct_t *data;
    int status;
    data = malloc(sizeof(my_struct_t));
    if (data == NULL)
        printf("Allocate structure\n");
    status = pthread_mutex_init(&data->mutex, NULL);
    if (status != 0)
        printf("Init mutex with status %d\n", status);
    status = pthread_mutex_destroy(&data->mutex);
    if (status != 0)
        printf("Destroy mutex %d\n", status);
    (void)free(data);
    return status;
}