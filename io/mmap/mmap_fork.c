#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#define BUF_SIZE        1024
/**
 * 进程间的共享内存. 子进程写数据, 夫进程读数据. 
 * 
 * 
 * @return int 
 */
int main()
{

    char *pstr = NULL;
    pid_t pid;

    pstr = mmap(NULL, BUF_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS , -1, 0);

    if (pstr == MAP_FAILED) {
        perror("mmap()");
        exit(-1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(-1);
    }

    if (pid > 0) {//child process
        strcpy(pstr, "hi, mmap");
        exit(0);
    } 
    else { //parent process

        wait(NULL);
        printf("pstr=%s\n", pstr);
    }

    munmap(pstr, BUF_SIZE);
}