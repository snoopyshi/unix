#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    opterr = 0;
    int ret;
    while ( (ret = getopt(argc, argv, "a:bcd")) != -1)
    {
        switch (ret)
        {
        case 'a':
            /* code */
            printf("ret=%c, optarg=%s\n", ret, optarg);
            break;
        
        default:
            printf("no impl\n");
            break;
        }
    }

    return 0;
}