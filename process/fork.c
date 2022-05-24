#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>



int main()
{

    pid_t pid;

    printf("[%d]Begin!\n", getpid());

    pid = fork();
    if ( pid < 0 )
    {
        perror("fork()");    
        return 0;
    }

    if (pid == 0) 
    {
        printf("[%d]:Children is working !\n", getpid());
    } else 
    {
        printf("[%d]: parent is working !\n", getpid());
    }

    printf("[%d] end\n", getpid());

    return 0;
}