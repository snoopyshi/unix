#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS     3
#define TCOUNT          10
#define COUNT_LIMIT     12

int count = 0;
pthread_mutex_t count_mutex;//锁
pthread_cond_t count_threshold_cv;//信号量

static void* inc_count(void* argv) 
{
    sleep(1);
    long my_id = (long) argv;
    int i;
    for (i = 0; i < TCOUNT; i++) {
        pthread_mutex_lock(&count_mutex);
        count++;
        if (count == COUNT_LIMIT) {
            printf("inc_count(): thread %ld, count=%d threshold reached. ", my_id, count);
            pthread_cond_signal(&count_threshold_cv);
            printf("Just sent signal.\n");
        }
        printf("inc_count(): thread %ld, count=%d, unlocking mutex\n", my_id, count);
        pthread_mutex_unlock(&count_mutex);
        sleep(1);
    }

    pthread_exit(NULL);
}

static void* watch_count(void* argv)
{

    long my_id = (long) argv;

    printf("Starting watch_count(): thread %ld\n", my_id);

    pthread_mutex_lock(&count_mutex);
    while (count < COUNT_LIMIT) {
        printf("watch_count(): thread %ld Count=%d. Going into wait...\n", my_id, count);
        //注意这里 wait 后, 会自动 release mutex. 当接受到信号的时候, 能自动获得 mutex
        //另外, pthread_cond_wait 中, condition 和 mutex 配套使用.
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
        printf("watch_count(): thread %ld Condition signal received. Count=%d\n", my_id, count);
    }

    printf("watch_count(): thread %ld Updating the value of count...\n", my_id);
    count += 125;
    printf("watch_count(): thread %ld count now=%d\n", my_id, count);
    printf("watch_count(): thread %ld Unlock mutex\n", my_id);
    pthread_mutex_unlock(&count_mutex);

    pthread_exit(NULL);
}


/**
 * 
 * This simple example code demonstrates the use of several Pthread condition variable routines.
 * The main routine creates three threads.
 * Two of the threads perform work and update a “count” variable.
 * The third thread waits until the count variable reaches a specified value.
 * 
 * @return int 
 */
int main()
{

    int i, rc;
    long t1 = 1, t2 = 2, t3 = 3;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    pthread_create(&threads[0], &attr, watch_count, (void*)t1);
    pthread_create(&threads[1], &attr, inc_count, (void*)t2);
    pthread_create(&threads[2], &attr, inc_count, (void*)t3);

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main(): waited and joined with %d threads. Final value of count=%d. Done.\n", NUM_THREADS, count);

    /**销毁动作**/
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);

    return 0;
}