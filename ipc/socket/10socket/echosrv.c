#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


/**退出宏**/
#define ERR_EXIT(m)             \
        do                      \
        {                       \
            perror(m);          \
            exit(-1);           \
        } while(0)

struct packet
{
    int len;
    char buf[1024];
};

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

/**
 * 执行入口
 *
 * */
int main(void)
{
    int listenfd;

    listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    /**listenfd = socket(PF_INET, SOCKET_STREAM, 0)**/
    if(listenfd < 0) 
        ERR_EXIT("socket");

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /**本机的地址, 本机的地址长度**/
    if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }

    if (listen(listenfd, SOMAXCONN)) {
        ERR_EXIT("listen");
    }
    

    struct sockaddr peeraddr;
    socklen_t peerlen = sizeof(peeraddr);

    int conn;
    /**对等方的地址, 对等方的地址长度**/
    if ((conn = accept(listenfd, &peeraddr, &peerlen)) < 0 )
        ERR_EXIT("accept()");

    //输出对等方的链接地址信息
    struct sockaddr_in* addr_in = (struct sockaddr_in*) &peeraddr;
    printf("client: ip=%s, port=%d\n", inet_ntoa(addr_in->sin_addr), ntohs(addr_in->sin_port));


    struct packet recvbuf;

    while (1)
    {
        memset(&recvbuf, 0, sizeof(recvbuf));
        int ret = readn(conn, &recvbuf.len, sizeof(int));
        if (ret == -1) 
        {
            break;
        } else if (ret < 4) 
        {
            printf("peer close(): read len;");
            break;
        }

        int n = ntohl(recvbuf.len);
        ret = readn(conn, &recvbuf.buf, n);
        if ( ret == -1 ) {
            break;
        }
        else if (ret < n) {
            printf("peer close():read data;");
        }
        fputs(recvbuf.buf, stdout);
        writen(conn, &recvbuf, 4 + n);
    }

    close(conn);
    close(listenfd);

    return 0;
}
