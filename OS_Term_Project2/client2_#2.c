#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP "220.149.128.100"
#define SERV_PORT 4101

int main(int argc, char *argv[])
{
	int sockfd, new_fd = 0;
	struct sockaddr_in dest_addr;

	int rcv_byte;
	char buf[512];

	char id[20];
	char pw[20];
	char msg[512];
	char msg1[512];
	char fd[20];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd == -1) {
		perror("Client-socket() error lol!");
		exit(1);
	}
	else printf("Client-socket() is OK...\n");

	dest_addr.sin_family = AF_INET;

	dest_addr.sin_port = htons(SERV_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(SERV_IP);

	memset(&(dest_addr.sin_zero), 0, 8);

	if(connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1) {
		perror("Client-connect() error lol!");
		exit(1);
	}
	else printf("Client-connect() is OK...\n\n");

	rcv_byte = recv(sockfd, buf, sizeof(buf), 0);
	printf("%s\n", buf);

	printf("ID : ");
	scanf("%s", id);
	send(sockfd, id, strlen(id) + 1, 0);

	printf("PW : ");
	scanf("%s", pw);
	send(sockfd, pw, strlen(pw) + 1, 0);

	if(!strcmp(id, "user2")) 	//Comparing recieved id whether user2 or not
		if(!strcmp(pw, "passwd2")) { 	//Comparing recieved pw whether passwd2 or not
			recv(sockfd, msg, sizeof(msg), 0);
			printf("%s\n", msg);
			memset(msg, 0, 8);
			pid_t pid;
			pid = fork();
			while(1) {
				if(pid == 0) {
					printf(">> ");
					scanf("%s", msg);
					send(sockfd, msg, strlen(msg) + 1, 0);
					if(!strcmp(msg, "exit")) {
						exit(1);
					}
				}
				else {
					if(rcv_byte = recv(sockfd, msg1, sizeof(msg1), 0) >= 0) {
						printf("[user1] %s\n>> ", msg1);
						memset(msg1, 0, 8);
					}
				}
			}
		}
		else exit(1);	

	return 0;
}
