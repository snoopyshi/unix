
#ifndef _RELAYER_H__
#define _RELAYER_H__

#include <sys/types.h>

enum 
{
    STATE_RUNNING = 1,
    STATE_CANCELED,
    STATE_OVER
};

struct rel_stat_st 
{
    int state;
    int fd1;
    int fd2;
    int64_t count12, count21;
};

/**
 * 添加任务
 * 
 * @param fd1 
 * @param fd2 
 * @return int 
 * return >= success
 */
int rel_addjob(int fd1, int fd2);


/**
 * 取消任务
 * 
 * @param id 
 * @return int 
 * return == 0  success
 */
int rel_canceljob(int id);


/**
 * @brief 
 * 
 * @param id 
 * @return int 
 * return 0  success, 并返回状态
 */
int rel_waitjob(int id, struct rel_stat_st *p);


/**
 * @brief 
 * 
 * @param id 
 * @param p 
 * @return int
 * 0 success 指定任务状态返回
 *  
 */
int rel_statjob(int id, struct rel_stat_st *p);


#endif