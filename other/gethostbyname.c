#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>


int main()
{
    char buf[1024];
    struct hostent *host;
    host = gethostbyname("gz881");
    if (NULL == host)
    {
        fprintf(stderr, "gethostbyname() h_error=%d\n", h_errno);
        exit(-1);
    }

    printf("h_name=%s\n", host->h_name); 
    printf("h_aliases=%s\n", *(host->h_aliases));
    printf("h_addrtype=%d\n", host->h_addrtype);
    printf("h_length=%d\n",host->h_length);
    printf("h_addr_list=%s\n",*(host->h_addr_list));

    return 0;
}