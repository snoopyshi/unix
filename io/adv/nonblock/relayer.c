#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define TTY1        "/dev/tty11"
#define TTY2        "/dev/tty12"
#define BUFSIZE     1024
enum 
{
    STATE_R=1,//写态
    STATE_W,//读态
    STATE_EX,//退出态
    STATE_T//终止态
};
/**有限状态机**/
struct fsm_st
{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int len;
    int pos;
    char* errstr;
};

void fsm_driver(struct fsm_st *fsm)
{
    int ret;
    switch (fsm->state)
    {
    case STATE_R:
        fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
        /* code */
        if (fsm->len == 0) {
            fsm->state = STATE_T;//将状态推动到 STATE_T 态
        } else if (fsm->len < 0) {
            if (errno == EAGAIN) 
                fsm->state = STATE_R;
            else {
                fsm->state = STATE_EX;
                fsm->errstr = "read()";
            }
        } else {
            fsm->pos = 0;
            fsm->state = STATE_W;
        }
        break;
    case STATE_W:
        /* code */
        ret = write(fsm->dfd, fsm->buf, fsm->len+fsm->pos);
        if(ret < 0) {
            if (errno == EAGAIN) {
                fsm->state = STATE_W;
            } else {
                fsm->errstr = "write()";
                fsm->state = STATE_EX;
            }
        } else {
            fsm->pos += ret;
            fsm->len -= ret;
            if (fsm->len == 0) {
                fsm->state = STATE_R;
            } else {
                fsm->state = STATE_W;
            }
        }
        break;    
    case STATE_EX:
        perror(fsm->errstr);
        fsm->state = STATE_T;
        /* code */
        break;
    case STATE_T:
        /**do something**/
        /* code */
        break;    
    default:
        break;
    }
}

void relayer(int fd1, int fd2)
{
    struct fsm_st fsm12, fsm21;

    int fd1_save, fd2_save;
    fd1_save = fcntl(fd1, F_GETFL);
    fcntl(fd1, fd1_save|O_NONBLOCK);

    fd2_save = fcntl(fd2, F_GETFL);
    fcntl(fd2, fd2_save|O_NONBLOCK);
    
    /**状态机初始化**/
    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    /**开始状态扭转**/
    while (fsm12.state != STATE_T || fsm21.state != STATE_T)
    {
        fsm_driver(&fsm12);
        fsm_driver(&fsm21);
    }


    /**恢复现场, 外面是啥状态, 回去还是啥状态**/
    fcntl(fd1, fd1_save);
    fcntl(fd2, fd2_save);
}

int main()
{
    int fd1, fd2;
    fd1 = open(TTY1, O_RDWR);
    if (fd1 < 0) {
        perror("open()");
        exit(-1);
    }
    write(fd1, "tty11\n", 5);

    fd2 = open(TTY2, O_RDWR|O_NONBLOCK);
    if (fd2 < 0) {
        perror("open()");
        exit(-1);
    }
    write(fd2, "tty12\n", 5);
    
    relayer(fd1, fd2);

    close(fd1);
    close(fd2);
    return 0;
}

