#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS     5

static void* func(void* i)
{
    int t = *((int*)i);
    printf("thread:%d\n", t);
    pthread_exit(NULL);
}

int main()
{
    int i;
    int* taskids[NUM_THREADS];
    pthread_t tids[NUM_THREADS];

    for (i = 0; i < NUM_THREADS; i++) {
        
        taskids[i] = malloc(sizeof(int));
        *taskids[i] = i;
        printf("Creating thread:i=%d taskid=%d\n", i, *(taskids[i]));
        pthread_create(tids+i, NULL, func, taskids[i]);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);//回收
    }

    return 0;
}