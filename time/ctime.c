#include <stdio.h>
#include <time.h>


int main()
{

	time_t t;
	
	if (time(&t) < 0) {
		perror("time()");		
	}
	
	printf("current time %s", ctime(&t));


	return 0;
}



