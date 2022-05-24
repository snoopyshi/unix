#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


#define NUM_THREADS     5

struct thread_data
{
    int threadid;
    int sum;
    char* message;
};

static void* func(void* i)
{
    struct thread_data *data = (struct thread_data*)i;

    printf("thread:sum=%d, threadid=%d, msg=%s\n", data->sum, data->threadid, data->message);
    pthread_exit(NULL);
}

int main()
{
    int i;
    //线程数据
    struct thread_data tdata_arr[NUM_THREADS];

    pthread_t tids[NUM_THREADS];

    for (i = 0; i < NUM_THREADS; i++) {
        char* message = malloc(sizeof(1024));
        sprintf(message, "thread message %d", i);
        tdata_arr[i].sum = i;
        tdata_arr[i].message = message;
        tdata_arr[i].threadid = i;
        
        printf("Creating thread:i=%d\n", i);
        pthread_create(tids+i, NULL, func, &tdata_arr[i]);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);//回收
    }

    return 0;
}