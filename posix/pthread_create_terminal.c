#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS     5

/**
 * 
 * This simple example code creates 5 threads with the pthread_create() routine. 
 * Each thread prints a "Hello World!" message, and then terminates with a call to pthread_exit().
 * 
 * 注意这里的线程参数传递.是一种不好的实现.
 * 
 * @param threadid 
 * @return void* 
 */

static void* PrintHello(void *threadid)
{
    int tid;
    tid = (int) threadid;
    fprintf(stdout, "Hello World! It's me, thread %d!\n", tid);

    pthread_exit(NULL);
}

int main()
{
    int i, rc;
    pthread_t threads[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; i++) {
        fprintf(stdout, "In main: creating thread %d\n", i);
        rc = pthread_create(&threads[i], NULL, PrintHello, (void*)i);
        if (rc) {
            fprintf(stdout, "Error: pthread_create():%s\n", strerror(rc));
            exit(-1);
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);//回收
    }

    return 0;
}