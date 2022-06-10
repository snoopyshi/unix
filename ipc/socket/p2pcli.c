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
#define ERR_EXIT(m) \
        do \
        {\
            perror(m);\
            exit(-1);\
        } while(0)
        

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

    pid_t pid;
    pid = fork();
    if (pid == -1)
        ERR_EXIT("fork()");

    if (pid == 0) {
        //child read:
        char recvbuf[1024] = {0};
        int ret;
        while (1)
        {
            ret = read(sock, recvbuf, sizeof(recvbuf));
            if (ret == -1)
                ERR_EXIT("read()");
            else if (ret == 0)
            {
                printf("peer close()\n");
                break;
            }
            fputs(recvbuf, stdout);
        }
        close(sock);
    } else 
    {
        char sendbuf[1024] = {0};
        int ret;
        while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
        {
            ret = write(sock, sendbuf, strlen(sendbuf));
            memset(sendbuf, 0, sizeof(sendbuf));
        }
    }
    
    return 0;
}