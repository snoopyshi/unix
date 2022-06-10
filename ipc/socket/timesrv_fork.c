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


static void do_recv(int conn, const char *ip, const int port)
{

    //处理客户端请求
    char readbuf[4];
    time_t curt;
    while (1)
    {
        read(conn, readbuf, sizeof(readbuf));
        time(&curt);
        write(conn, ctime(&curt), 1024);
        printf("client:%s, %d input:%s\n", ip, port, readbuf);
    }
    close(conn);
}

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
    pid_t pid;

    while (1)
    {
        int conn = accept(listenfd, (struct sockaddr*)&peeraddr, &addrlen);//接收
        if (conn < 0) 
            ERR_EXIT("conn()"); 
        char *ip = inet_ntoa(peeraddr.sin_addr);
        int cport = ntohs(peeraddr.sin_port);
        printf("client login: ip=%s, port=%d\n", ip, cport);

        pid = fork();
        if (pid == -1) {
            ERR_EXIT("fork");
        }

        if (pid == 0) {
            //child process
            do_recv(conn, ip, cport);
            exit(EXIT_SUCCESS);
        }
    }

    close(listenfd);

    return 0;
}