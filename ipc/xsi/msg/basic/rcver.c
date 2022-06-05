#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>


#include "proto.h"

/**
 * @brief 消息队列的接收方
 * 
 * @return int 
 */
int main()
{

    key_t key;
    int msgid;
    struct msg_st rbuf;

    key = ftok(KEYPATH, KEYPROJ);

    if (key < 0) {
        perror("ftok()");
        exit(-1);
    }

    msgid = msgget(key, IPC_CREAT|0600);
    if (msgid < 0) {
        perror("msgget()");
        exit(-1);
    }

    while (1) 
    {
        if (msgrcv(msgid, &rbuf, sizeof(rbuf) - sizeof(long), 0, 0) < 0) {
            perror("msgrcv()");
            exit(-1);
        }
        printf("name=%s\t", rbuf.name);
        printf("math=%d\t", rbuf.math);
        printf("chinese=%d\n", rbuf.chinese);
    }
    

    msgctl(msgid, IPC_RMID, NULL);//销毁当前实例

    return 0;
}