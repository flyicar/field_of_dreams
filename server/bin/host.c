#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SENDLINE_SIZE 512
#define RECVLINE_SIZE 512

int main() {
   
    int fd[2];
    char filename[] = "a.fifo";
	char sendline[SENDLINE_SIZE], recvline[RECVLINE_SIZE];
    size_t size;

    fd[1] = open(filename, O_WRONLY);
    if (fd[1] == -1) {
		perror("open");
        exit(-1);
    }
	
	for (;;) {

		printf("Enter any command\n");
		fgets(sendline, sizeof(sendline), stdin);
		size = write(fd[1], sendline, sizeof(sendline));
		if (size != sizeof(sendline)) {
			perror("write");
			close(fd[1]);
			exit(1);
		}

	}

    close(fd[1]);

    return 0;
}

