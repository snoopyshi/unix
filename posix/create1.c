#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void *func(void *p)
{
    puts("hello world");

    pthread_exit(NULL);
    // return NULL;
}

int main()
{

    pthread_t tid;
    int err;

    printf("Begin!\n");
    err = pthread_create(&tid, NULL, func, NULL);
    if (err) 
    {
        fprintf(stderr, "pthread_create fail: %s\n", strerror(err));
        exit(1);
    }

    pthread_join(tid, NULL);
    printf("End!\n");
    return 0;
}
