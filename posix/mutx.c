#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * pthread_mutex_init (mutex,attr)
 * pthread_mutex_destroy (mutex)
 * pthread_mutexattr_init (attr)
 * pthread_mutexattr_destroy (attr)
 * 
 * @return int 
 */
int main()
{
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
    pthread_mutex_init(&mutex, &attr);

    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&attr);


    return 0;
}