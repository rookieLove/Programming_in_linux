#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	if(argc < 2) {	
		printf("a.out filename\n");
	}

	int ret = access(argv[1], W_OK);
//      int ret = access(argv[0], W_OK);
	if(ret == -1) {
		perror("access");
		exit(1);
	}
	printf("you can write this file.\n");
	return 0;
}
