#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FAILED != 0

void	*print_hello(void *void_thread_id_ptr)
{
	unsigned int    *thread_id_ptr;
    pthread_t       sub_thread;

	thread_id_ptr = (unsigned int *)void_thread_id_ptr;
    sub_thread = pthread_self();
	printf("Hello from id:%d\n", *thread_id_ptr);
    if(sub_thread)
        printf("Hello from id:%d son of %d\n", (unsigned int)sub_thread, *thread_id_ptr);
    pthread_exit(NULL);
	return (NULL);
}

int	main(void)
{
	int			i;
	int			rc;
	pthread_t	thread_id;

	i = 0;
	while (i++ < 3)
	{
		if (pthread_create(&thread_id, NULL, print_hello, (void *)&thread_id) FAILED)
		{
			printf("error creating thread\n");
			exit(1);
		}
	}
	pthread_exit(NULL);
	return (0);
}
