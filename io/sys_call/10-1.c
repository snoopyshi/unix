#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/**
 *	下面程序输出什么?
 *
 * */
int main()
{
	int fd1, fd2;
	int i;
	for (i = 0; i < 100; i++ ) 
	{
		fd1 = open("foo1.txt", O_RDONLY, 0);	
		close(fd1);
	}

	fd2 = open("foo2.txt", O_RDONLY, 0);
	printf("fd2 = %d\n", fd2);//0 1 2 默认打开的标准输入输出, 错误输出. 所以这里应该是输出3
	exit(0);
}



