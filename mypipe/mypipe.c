#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#include "mypipe.h"


/**数据结构**/
struct mypipe_t
{
    int head;
    int tail;
    char data[PIPESIZE];//内存空间
    int datasize;
    int count_rd, count_wd;
    pthread_mutex_t mut;//独占的形式来使用
    pthread_cond_t cond;//信号量
};

/**初始化**/
mypipe_t *mypipe_init()
{
    struct mypipe_t *me;
    me = malloc(sizeof(*me));
    if (me == NULL) {
        return NULL;
    }

    me->head = 0;
    me->tail = 0;
    me->datasize = 0;
    me->count_rd = 0;
    me->count_wd = 0;
    pthread_mutex_init(&me->mut, NULL);
    pthread_cond_init(&me->cond, NULL);

    return me;
}

int mypipe_register(mypipe_t *ptr, int opmap)
{
    struct mypipe_t* me = ptr;
    pthread_mutex_lock(&me->mut);
    if (opmap & MYPIPE_READ) {
        me->count_rd++;
    }
    if (opmap & MYPIPE_WRITE) {
        me->count_wd++;
    }
    pthread_cond_broadcast(&me->cond);
    while (me->count_rd <=0 || me->count_wd <= 0)
    {
        pthread_cond_wait(&me->cond, &me->mut);
    }

    pthread_mutex_unlock(&me->mut);
    return 0;
}

int mypipe_unregister(mypipe_t *ptr, int opmap)
{
    struct mypipe_t *me = ptr;
    pthread_mutex_lock(&me->mut);

    if (opmap & MYPIPE_READ) 
        me->count_rd--;
    if (opmap & MYPIPE_WRITE)
        me->count_wd--;

    pthread_cond_broadcast(&me->cond);
    

    pthread_mutex_unlock(&me->mut);
}

static int mypipe_readbyte_unlocked(struct mypipe_t *me, char* datap) 
{
    if (me->datasize <= 0) {
        return -1;
    }
    *datap = me->data[me->head];
    me->head = next(me->head);
    me->datasize--;
    return 0;
}

int mypipe_read(mypipe_t *ptr, void* buf, size_t count)
{
    int i;
    struct mypipe_t *me = ptr;
    pthread_mutex_lock(&me->mut);

    while (me->datasize <= 0 && me->count_wd > 0)
        pthread_cond_wait(&me->cond, &me->mut);
    
    if (me->datasize <= 0 && me->count_wd <= 0) {
        pthread_mutex_unlock(&me->mut);
        return 0;
    }

    for (i = 0; i < count; i++) {
        if (mypipe_readbyte_unlocked(me, buf+i) < 0) {
            break;
        }
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return  i;
}

int mypipe_write(mypipe_t *ptr, const void* buf, size_t size)
{

}

/**销毁**/
int mypipe_destroy(mypipe_t* ptr)
{
    struct mypipe_t *me = ptr;
    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(ptr);

    return 0;
}