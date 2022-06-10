#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>




/**退出宏**/
#define ERR_EXIT(m)             \
        do                      \
        {                       \
            perror(m);          \
            exit(-1);           \
        } while(0)
        

void handler(int sig)
{
    printf("recv sig %d\n", sig);
    exit(EXIT_SUCCESS);
}

/**
 * @brief 
 * 点对点的聊天
 * 
 * @return int 
 */
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

    int conn;
    pid_t pid;
    /**对等方的地址, 对等方的地址长度**/
    if ((conn = accept(listenfd, &peeraddr, &peerlen)) < 0 )
        ERR_EXIT("accept()");

    pid = fork();

    if (pid == -1) 
        ERR_EXIT("fork()");

    if (pid == 0) {
        //child process: read
        char recvbuf[1024];
        while (1)
        {
            memset(recvbuf, 0, sizeof(recvbuf));
            int ret = read(conn, recvbuf, sizeof(recvbuf));//阻塞住了.
            if (ret == -1) 
                ERR_EXIT("read()");
            else if (ret == 0)
            {
                printf("peer close\n");
                break;
            }
            fputs(recvbuf, stdout);
        }
        close(conn);
        //通知父进程关闭
        kill(getppid(), SIGUSR1);
        exit(EXIT_SUCCESS);

    } else 
    {
        //parent process write
        signal(SIGUSR1, handler);

        char sendbuf[1024];
        while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL) //阻塞住了.
        {
            int ret = write(conn, sendbuf, strlen(sendbuf));
            if (ret == -1) 
                ERR_EXIT("write()");
            memset(sendbuf, 0, sizeof(sendbuf));
        }
        printf("parent close()");
        close(listenfd);
        exit(EXIT_SUCCESS);
    }

    return 0;
}