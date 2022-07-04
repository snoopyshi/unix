#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "robust_io.h"

ssize_t rio_readn(int fd, void *buf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = buf;

    while (nleft > 0) 
    {
        
    }
}

ssize_t rio_writen(int fd, const void *buf, size_t count)
{



}