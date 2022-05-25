#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main()
{

    pthread_cond_t cond;

    pthread_cond_init(&cond, NULL);

    pthread_cond_destroy(&cond);

    return 0;
}
