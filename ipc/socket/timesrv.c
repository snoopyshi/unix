#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/**退出宏**/
#define ERR_EXIT(msg) \
        do \
        {\
            perror(msg);\
            exit(-1);\
        } while(0)

/**
 * @brief 时间服务器
 * 
 * @return int 
 */
int main()
{
    int listenfd;
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket()");

    struct sockaddr_in servaddr;//需要初始化地址
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5199);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on , sizeof(on));

    if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr))) 
        ERR_EXIT("bind()");

    if (listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen()");

    struct sockaddr_in peeraddr;//对等方的地址.
    socklen_t addrlen = sizeof(peeraddr);
    int conn = accept(listenfd, (struct sockaddr*)&peeraddr, &addrlen);//接收
    if (conn < 0) 
        ERR_EXIT("conn()"); 

    printf("client: ip=%s, port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

    //处理客户端请求
    char readbuf[4];
    time_t curt;
    while (1)
    {
        read(conn, readbuf, sizeof(readbuf));
        time(&curt);
        write(conn, ctime(&curt), 1024);
        printf("cli input:%s\n", readbuf);
    }

    close(conn);
    close(listenfd);

    return 0;
}