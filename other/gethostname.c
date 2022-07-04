#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char buf[1024];
    int ret;
    ret = gethostname(buf, 1024);
    if (ret) {
        perror("gethostname()");
        exit(-1);
    }

    printf("hostname=%s", buf);
    printf("kk\n");

    return 0;
}