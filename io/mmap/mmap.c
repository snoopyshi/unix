#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


/**
 * @brief mmap base example
 * 
 * @return int 
 */
int main(int argc, char *argv[])
{
	int fd, i;
	size_t len;
	char *pstr = NULL;
	struct stat buf;
	int count = 0;
	if (argc < 2)
	{
		printf("Usage ...\n");
		exit(-1);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open()\n");
		exit(-1);
	}
	
	fstat(fd, &buf);

	pstr = mmap(pstr, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	
	if (MAP_FAILED == pstr) {
		perror("mmap()");
		exit(-1);
	}

	for (i = 0; i < buf.st_size; i++) {
		if (*(pstr+i) == 'a') {
			count++;
		}
	}

	printf("count=%d\n", count);

	close(fd);
	munmap(pstr, buf.st_size);

	return 0;
}
