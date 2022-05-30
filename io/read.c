#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char **argv)
{

    char buf[1024];
    ssize_t ret;
    char* path = argv[1];

    int fd = open(path, O_RDONLY);

    while (1)
    {
        ret = read(fd, buf, 1024);
        printf("buf=%s\n",buf);
    }

    return 0;
}