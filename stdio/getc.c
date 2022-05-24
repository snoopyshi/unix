#include <stdio.h>
#include <stdlib.h>

int  main()
{

    int c;
    char buf[1];

    while ( (c = getc(stdin)) != EOF) {
        printf("c=%d\n", c);
        // if (putc(c, stdout) == EOF)
        //     printf("output error\n");

    } 
        


    return 0;

}
