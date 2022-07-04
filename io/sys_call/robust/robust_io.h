#ifndef __RO_BUST_H

#define __RO_BUST_H

#include <unistd.h>

ssize_t rio_readn(int fd, void *buf, size_t count);


ssize_t rio_writen(int fd, const void *buf, size_t count);

#endif