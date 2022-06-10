#include <stdio.h>
#include <arpa/inet.h>

/**
 * @brief 字节序转换函数
 * 
 * @return int 
 */
int main()
{

    unsigned int x = 0x12345678;//四个字节

    unsigned int y = htonl(x);

    unsigned char* p = (unsigned char*)&y;

    printf("%0x %0x %0x %0x\n", p[0],p[1],p[2],p[3]);

    return 0;
}