#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SENDLINE_SIZE 512
#define RECVLINE_SIZE 512

int main(int argc, char *argv[]) {
	
	int sockfd;
	char sendline[SENDLINE_SIZE], recvline[RECVLINE_SIZE];
	struct sockaddr_in servaddr;
	size_t size;
	
	if (argc != 3) {
		printf("Usage: tcp_cl <IP address> <port>\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket");
		exit(1);
	}

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	if (inet_aton(argv[1], &servaddr.sin_addr) == 0) {
		perror("inet_aton");
		close(sockfd);
		exit(1);
	}

	if (connect(sockfd, (struct sockaddr *) &servaddr, 
	sizeof(servaddr)) < 0) {
		perror("connect");
		close(sockfd);
		exit(1);
	}

	//for (;;) {

		printf("Enter any string:\n");
		fgets(sendline, sizeof(sendline), stdin);

		size = write(sockfd, sendline, sizeof(sendline));
		if (size != sizeof(sendline)) {
			perror("write");
			close(sockfd);
			exit(1);
		}
		
		for (;;) {

			printf("Waiting command:\n");
			size = read(sockfd, recvline, sizeof(recvline));
			if (size < 0) {
				perror("read");
				close(sockfd);
				exit(1);
			}

			printf("Response from server:\n");
			printf("%s\n", recvline);
		}

	close(sockfd);

	return 0;
}
