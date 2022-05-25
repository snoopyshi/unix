#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS         10
double sum = 0;
//初始化1
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//静态方法初始化
//初始化2
pthread_mutex_t mutex;
void* counting(void* p)
{
    int i;
    int tid = (int) p;
    //方案一: 
    for (i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&mutex);
        sum++;
        pthread_mutex_unlock(&mutex);
    }
    // 
    printf("End: tid=%d\n", tid);
    pthread_exit(NULL);
}
/**
 * @brief 
 * 
 * @return int 
 */
int main()
{
    int i;
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, counting, (void*)i);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    
    printf("Main: sum=%f\n", sum);
    return 0;
}