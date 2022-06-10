#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>



int main()
{
    unsigned long addr = inet_addr("192.168.0.100");

    struct in_addr ipaddr;
    ipaddr.s_addr = addr;
    printf("%s\n", inet_ntoa(ipaddr));//网络序, 需要转成本机序

    printf("%ld\n", ntohl(addr));//这个 addr 是网络字节序

    printf("%u\n", ntohl(addr));//这个 addr 是网络字节序


    return 0;
}