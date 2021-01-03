#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP	"220.149.128.100"
#define SERV_PORT	4130
#define BACKLOG	10

#define INIT_MSG	"=======================\nHello! I'm P2P File Sharing Server...\nPlease, LOG-IN!\n======================\n"
#define	Log_in_user1	"Log-in success!! [user1] *^^*\n"
#define Log_in_user2	"Log-in success!! [user2] *^^*\n"
#define Log_in_fail	"Log-in fail: Incorrect password...\n"	
#define USER1_ID	"user1"
#define USER1_PW	"passwd1"
#define USER2_ID	"user2"
#define USER2_PW	"passwd2"

int main(int argc, char *argv[ ])
{
	/* listen on sock_fd, new connection on new_fd */
	int sockfd, new_fd;

	/* my address information, address where I run this program */
	struct sockaddr_in my_addr;

	/* remote address information */
	struct sockaddr_in their_addr;
	unsigned int sin_size = sizeof(struct sockaddr_in);
	
	/* fork declare */
	pid_t pid;
	
	/* return value from strcmp */
	int ret_user1_ID;
	int ret_user1_PW;	
	int ret_user2_ID;
	int ret_user2_PW;
	 

	/* buffer */
	int rcv_byte;
	char buf[512];

	char id[20];
	char pw[20];

	char msg[512];

	int val = 1;

	/* socket */
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd == -1){
		perror("Server-socket() error lol!");
		exit(1);
	}
	else printf("Server-socket() sockfd is OK...\n");

	/* host byte order */
	my_addr.sin_family = AF_INET;

	/*short, network byte order */
	my_addr.sin_port = htons(SERV_PORT);
	
	//my_addr.sin_addr.s_addr = inet_addr(SERV_IP);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* zero the rest of the struct */
	memset(&(my_addr.sin_zero),0,8);

	/* to prevent 'Address already in use...' */
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val))< 0)
	{
		perror("setsockopt");
		close(sockfd);
		return -1;
	}

	/* bind */
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("Server-bind() error lol!");
		exit(1);
	}
	else printf("Server-bind() is OK...\n");

	/* listen */
	if(listen(sockfd, BACKLOG) == -1)
	{
		perror("listen() error lol!");
		exit(1);
	}
	else printf("listen() is OK...\n\n");


	while(1)
	{
		/* ...other codes to read the received data... */
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		
		/* accept */
		if(new_fd == -1)
		{
			perror("accept() error lol!");
			exit(1);
		}
		else {
			printf("accept() is OK...\n\n");
		}

		pid = fork();
					
		/* fork */
		if(pid == 0){
			close(sockfd);
			/* Communicate with socket new_fd*/
			send(new_fd, INIT_MSG, strlen(INIT_MSG) + 1, 0);
			
			/* receive ID from Client*/
			rcv_byte = recv(new_fd, msg, sizeof(msg), 0);
			strcpy(id, msg);
			
			ret_user1_ID = strcmp(USER1_ID,id);
			ret_user2_ID = strcmp(USER2_ID,id);
			
			/* receive PW from Client*/
			rcv_byte = recv(new_fd, msg, sizeof(msg), 0);
			strcpy(pw, msg);

			ret_user1_PW = strcmp(USER1_PW,pw);
			ret_user2_PW = strcmp(USER2_PW,pw);
			
			/* ID, PW Verify */
			if(ret_user1_ID == 0 && ret_user1_PW == 0)
			{
				printf("==================================\n");
				printf("User Information\n");
				printf("ID: ");
				printf("%s", id);
				printf(", PW: ");
				printf("%s\n", pw);
				printf("%s\n",Log_in_user1);
				printf("===================================\n");	
				send(new_fd, Log_in_user1, strlen(Log_in_user1) + 1, 0);	
			}
			else if(ret_user2_ID == 0 && ret_user2_PW == 0)
			{
				printf("==================================\n");
				printf("User Information\n");
				printf("ID: ");
				printf("%s", id);
				printf(", PW: ");
				printf("%s\n", pw);
				printf("%s\n",Log_in_user2);
				printf("===================================\n");	
				send(new_fd, Log_in_user2, strlen(Log_in_user2) + 1, 0);
			}else{
				printf("==================================\n");
				printf("User Information\n");
				printf("ID: ");
				printf("%s", id);
				printf(", PW: ");
				printf("%s\n", pw);
				printf("%s\n",Log_in_fail);
				printf("===================================\n");	
				send(new_fd, Log_in_fail, strlen(Log_in_fail) + 1, 0);
			}
				close(new_fd);
				exit(0);
			}
		else{
			close(new_fd);
		}
	}
	return 0;
}
