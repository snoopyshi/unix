#include <stdio.h>

/**
 * @brief 字节序验证
 * 
 * centos 验证后是小端序 78 56 34 12
 * 
 * @return int 
 */
int main()
{

    unsigned int x = 0x12345678;
    unsigned char* p = (unsigned char*) &x;


    printf("%0x %0x %0x %0x\n", p[0], p[1], p[2], p[3]);
    printf("%d\n", x);

    return 0;
}