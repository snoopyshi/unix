#ifndef _MYPIPE__H
#define _MYPIPE__H

#define PIPESIZE        1024
#define MYPIPE_READ     0x00000001UL
#define MYPIPE_WRITE    0x00000002UL

typedef void mypipe_t;

/**初始化**/
mypipe_t *mypipe_init();

int mypipe_register(mypipe_t *, int opmap);

int mypipe_unregister(mypipe_t *, int opmap);

int mypipe_read(mypipe_t *p, void* buf, size_t size);

int mypipe_write(mypipe_t *p, const void* buf, size_t size);

/**销毁**/
int mypipe_destroy(mypipe_t *p);

#endif