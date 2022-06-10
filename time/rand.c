#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE    10

int main()
{
    int i;
    srand(time(NULL));

    for (i = 0; i < MAX_SIZE; i++)
    {
        printf("%d\n", rand() % 100);
    }

    return 0;
}