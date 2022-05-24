#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main()
{

    puts("Begin");

    pid_t pid = fork();

    if (pid == 0) {
        execl("/bin/date", "date", "+%s", NULL);
        perror("execl()");
        return 0;
    }

    wait(NULL);

    puts("End");
    return 0;
}