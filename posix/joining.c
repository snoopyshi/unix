#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#define NUM_THREADS    4

static void* BusyWork(void* t)
{
    int i;
    int tid;
    double result = 0.0;
    tid = *(int*)t;
    printf("Thread %d starting ...\n", tid);
    for (i = 0; i < 1000000; i++) {
        result = result + sin(i) * tan(i);//计算逻辑.
    }
    printf("Thread %d done. result=%e\n", tid, result);
    pthread_exit((void*) t);
}

int main()
{

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    void *status;
    int i, rc;
    int *tdata;
    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);//初始化线程属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < NUM_THREADS; i++) {
        printf("Main: creating thread %d\n", i);
        tdata = malloc(sizeof(int));
        *tdata = i;
        rc = pthread_create(&thread[i], &attr, BusyWork, tdata);
        if (rc) {
            fprintf(stderr, "Error: pthread_create(): %s\n", strerror(rc));
            exit(-1);
        }
    }

    //销毁线程属性
    pthread_attr_destroy(&attr);

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(thread[i], &status);
        if (rc) {
            printf("Error: return code from pthread_join(). %s\n", strerror(rc));
            exit(-1);
        }
        printf("Main: completed join with thread %d, status=%d\n", i, *(int*)status);
    }


    return 0;
}