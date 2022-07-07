#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>


/**退出宏**/
#define ERR_EXIT(m) \
        do \
        {\
            perror(m);\
            exit(-1);\
        } while(0)


ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;//剩余读取的长度
    ssize_t nread;//已经读取的字节
    char *readbuf = (char*) buf;
    while (nleft > 0)
    {
        nread = read(fd, readbuf, nleft);
        if (nread == -1) 
        {
            if (errno == EINTR) {
                continue;//被信号打断, 正常行为. 
            }
            return -1;//发生错误.
        } else if (nread == 0) 
        {
            //对等方关闭了. 读取了部分. 
            return count - nleft;
        }
        nleft -= nread;
        readbuf += nread;
    }
    return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;//剩余写的长度
    ssize_t nwritten;//已经写的字节
    char *writebuf = (char*) buf;
    while (nleft > 0)
    {
        nwritten = write(fd, writebuf, nleft);
        if (nwritten == -1) 
        {
            if (errno == EINTR) {
                continue;//被信号打断, 正常行为. 
            }
            return -1;//发生错误.
        } else if (nwritten == 0) 
        {
            continue;
        }
        nleft -= nwritten;
        writebuf += nwritten;
    }
    return count;
}  


ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while (1)
    {
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        if (ret == -1 && errno == EINTR) {
            //被信号打断的场景
            continue;
        }
        return ret;
    }
}

ssize_t readline(int fd, void *buf, size_t count)
{
    int ret;
    int nread;
    char *bufp = buf;
    int nleft = count;
    while (1)
    {
        ret = recv_peek(fd, bufp, nleft);
        if (ret < 0)
            return ret;/**读取错误*/
        else if (ret == 0)
            return ret;/**对方关闭*/

        nread = ret;
        int i;
        for (i = 0; i < nread; i++) {
            if (bufp[i] == '\n') {
                ret = readn(fd, bufp, i+1);
                if (ret != i+1) {
                    exit(EXIT_FAILURE);//失败退出
                }
                return ret;
            }
        }

        if (nread > nleft) {
            exit(EXIT_FAILURE);
        }
        nleft -= nread;
        ret = readn(fd, bufp, nread);
        if (ret != nread) {
            exit(EXIT_FAILURE);
        }

        bufp += nread;
    }
    
    return -1;
}

void echo_cli(int sock)
{
    /**
    int n;
    int ret;
    char sendbuf[1024];
    char recvbuf[1024];
    memset(sendbuf, 0, sizeof(sendbuf));
    memset(recvbuf, 0, sizeof(recvbuf));


    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        writen(sock, &sendbuf, strlen(sendbuf));

        ret = readline(sock, &recvbuf, sizeof(recvbuf));
        if (ret < 0) {
            break;
        } else if (ret == 0) {
            printf("peer close(): read len\n");
            break;
        }
    
        fputs(recvbuf, stdout);
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock);
    **/

    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};

    int nready;
    fd_set rset;
    FD_ZERO(&rset);
    int maxfd;
    int ret;
    int fd_stdin = fileno(stdin);
    if (fd_stdin < sock) {
        maxfd = sock;
    } else {
        maxfd = fd_stdin;
    }
    
    while (1)
    {
        FD_SET(fileno(stdin), &rset);
        FD_SET(sock, &rset);
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if (nready == -1) {
            ERR_EXIT("select()");
        }else if (nready == 0) {
            continue;
        }

        if (FD_ISSET(sock, &rset)) {
            ret = readline(sock, &recvbuf, sizeof(recvbuf));
            if (ret < 0) {
                break;
            } else if (ret == 0) {
                printf("server close()\n");
                break;
            }
            fputs(recvbuf, stdout);
            memset(&recvbuf, 0, sizeof(recvbuf));
        }

        if (FD_ISSET(fd_stdin, &rset)) {
            //
            if (fgets(sendbuf, sizeof(sendbuf), stdin) == NULL) {
                break;
            }
            writen(sock, &sendbuf, strlen(sendbuf));
            memset(&sendbuf, 0, sizeof(sendbuf));
        }
    }

    close(sock);

}

int main()
{
    int sock;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    /**listenfd = socket(PF_INET, SOCKET_STREAM, 0)**/
    if(sock < 0) 
        ERR_EXIT("socket");

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
        ERR_EXIT("connect");

    echo_cli(sock);
    
    return 0;
}