#include <stdio.h>
#include <time.h>


int main()
{
    time_t t;
    t = time(NULL);

    printf("second: %ld\n", t);

    return 0;
}