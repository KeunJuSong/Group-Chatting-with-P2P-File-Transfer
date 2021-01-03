#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP		"220.149.128.100"
#define SERV_PORT	4130

int main(int argc, char *argv[ ])
{
	int sockfd; /* will hold the destination addr */
	struct sockaddr_in dest_addr;

	int rcv_byte;
	char buf[512];

	char id[20];
	char pw[20];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd == -1)
	{
		perror("Client-socket() error lol!");
		exit(1);
	}
	else printf("Client-socket() sockfd is OK...\n");

	/* host byte order */
	dest_addr.sin_family = AF_INET;

	/* short, network byte order */
	dest_addr.sin_port = htons(SERV_PORT);
	dest_addr.sin_addr.s_addr = inet_addr(SERV_IP);

	/*zero the rest of the struct */
	memset(&(dest_addr.sin_zero),0,8);

	/* connect */
	if(connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("Client-connect() error lol");
		exit(1);
	}
	else printf("Client-connect() is OK...\n\n");

	/* receive INIT_MSG */
	rcv_byte = recv(sockfd, buf, sizeof(buf), 0);
	printf("%s\n", buf);
	
	/* input & send ID */
	printf("ID: ");
	scanf("%s", id);
	send(sockfd, id, strlen(id)+1,0);

	/* input & send pw */
	printf("PW: ");
	scanf("%s", pw);
	send(sockfd, pw, strlen(pw)+1,0);

	/* receive login result */
	rcv_byte = recv(sockfd, buf, sizeof(buf), 0);
	printf("%s", buf);
	
	close(sockfd);
	
	return 0;
}