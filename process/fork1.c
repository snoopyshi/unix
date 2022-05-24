#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define LEFT            300000000
#define RIGHT           300000200


int main()
{

    int i, j, mark;
    pid_t pid;
    for (i = LEFT; i < RIGHT ;i++) {
        pid = fork();
        if (pid == 0) {
            //子进程开始执行的代码
            mark = 0;
            for (j = 2; j < i/2; j++) {
                if (i%j == 0) {
                    mark = 1;
                    break;
                }
            }
            if (mark == 0) {
                printf("ppid=【%d】, pid=【%d】, value=%d\n", getppid(), getpid(), i);
            }
            sleep(10000);
            exit(0);
        }
        if (pid > 0) {
            //do nothing
            printf("parentid=【%d】, pid=【%d】\n", getpid(), pid);
        }
        if (pid == -1) {
            perror("fork error!\n");
        }
        
    }

    // for (i = LEFT; i < RIGHT ;i++) {
    //     wait(NULL);
    // }
    // sleep(100000);

    return 0;
}

