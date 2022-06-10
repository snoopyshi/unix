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
        

int main()
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

    listen(listenfd, SOMAXCONN);

    struct sockaddr peeraddr;
    socklen_t peerlen = sizeof(peeraddr);

    printf("waiting connection ...\n");

    int conn;
    /**对等方的地址, 对等方的地址长度**/
    if ((conn = accept(listenfd, &peeraddr, &peerlen)) < 0 )
        ERR_EXIT("accept()");

    char recvbuf[1024];
    while (1)
    {
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = read(conn, recvbuf, sizeof(recvbuf));
        puts(recvbuf);
        write(conn, recvbuf, ret);
    }

    close(conn);
    close(listenfd);

    return 0;
}