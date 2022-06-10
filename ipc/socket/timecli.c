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
    int sockfd;
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        ERR_EXIT("socket()");

    struct sockaddr_in servaddr;//需要初始化地址
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5199);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
        ERR_EXIT("connect()");

    //处理客户端请求
    char recvbuf[1024];
    unsigned int c;
    while ( (c = fgetc(stdin)) != EOF)
    {
        if ( c == '\n') {
            continue;
        } 
        write(sockfd, &c, sizeof(c));
        read(sockfd, recvbuf, sizeof(recvbuf));
        puts(recvbuf);
    }

    close(sockfd);

    return 0;
}