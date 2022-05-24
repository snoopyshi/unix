#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NTHREADS        4
#define N               1000
#define MEGEXTRA        1000000

//线程属性
pthread_attr_t attr;

static void* dowork(void* t)
{
    double A[N][N];
    int i, j;
    int tid;
    size_t mystacksize;

    tid = (int)t;
    pthread_attr_getstacksize(&attr, &mystacksize);
    printf("Thread %d: stack size=%li bytes \n", tid, mystacksize);
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = ((i*j)/3.452) + (N-i);
        }
    }
    pthread_exit(NULL);
}

int main()
{

    pthread_t threads[NTHREADS];
    //栈大小
    size_t stacksize;
    int rc;
    int i;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stacksize);
    printf("Default stack size = %li\n", stacksize);
    stacksize = sizeof(double) * N * N + MEGEXTRA;//重新分配的栈大小
    printf("Amount of stack needed per thread = %li\n", stacksize);
    pthread_attr_setstacksize(&attr, stacksize);

    printf("Creating threads with stack size = %li bytes\n",stacksize);
    for (i = 0; i < NTHREADS; i++) {
        rc = pthread_create(&threads[i], &attr, dowork, NULL);
        if (rc) {
            fprintf(stderr, "Error: pthread_create(): %s\n", strerror(rc));
            exit(-1);
        }
    }
    pthread_attr_destroy(&attr);

    for (i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }


    return 0;
}