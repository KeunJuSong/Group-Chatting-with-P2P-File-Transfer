#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP "220.149.128.100"
#define SERV_PORT 4130

#define BACKLOG 10

#define INIT_MSG "=========================\nHello! I'm P2P File Sharing Server...\nPlease, LOG_IN!\n=========================\n"
#define LOG_INF "\nLog-in Success! - Welcome to GTalk\n----------Chatting Room----------\n"
#define USER1_ID "user1"
#define USER1_PW "passwd1"
#define USER2_ID "user2"
#define USER2_PW "passwd2"

int main(void)
{
	int sockfd, new_fd, new_fd1 = 0, new_fd2 = 0;

	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	unsigned int sin_size;

	int rcv_byte = -1;
	int rcv_byte1;
	int rcv_byte2;
	char buf[512];
	char buf1[512];
	char buf2[512];

	char id[20];
	char pw[20];
	char rcv_fd[20];
	char rcv_fd1[20];
	char rcv_fd2[20];
	int fd[2];
	int fd1[2];
	int fd2[2];
	int id1[2];
	int pw1[2];

	char msg[512];
	char msg1[512];
	char msg2 [512];
	char msg3 [512];
	
	char CLIENT1_IP[20];
	char CLIENT2_IP[20];
	char CLIENT1_PORT_S[5];
	char CLIENT2_PORT_S[5];
	int CLIENT1_PORT;
	int CLIENT2_PORT;

	int val = 1;
	int state1 = 0;
	int state2 = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == 1) {
		perror("Server-socket() error lol!");
		exit(1);
	}
	else printf("Server-socket() sockfd is OK...\n");
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERV_PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), 0, 8);

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val)) < 0) {
		perror("setsockopt");
		close(sockfd);
		return -1;
	}

	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("Server-blind() error lol!");
		exit(1);
	}
	else printf("Server-blind() is OK...\n");

	if(listen(sockfd, BACKLOG) == -1) {
		perror("listen() error lol!");
		exit(1);
	}
	else printf("listen() is OK...\n\n");

	sin_size = sizeof(struct sockaddr_in);
		
	pid_t pid;
	pid_t pid1;//Declar fork varialble number
	pid_t pid2;
	pid_t pid3;
	pid = fork();//Call fork() function

	pipe(fd1);
	pipe(fd2);
	while(1) {
		if(pid == 0) {//Infinte loop
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		send(new_fd, INIT_MSG, strlen(INIT_MSG) + 1, 0);
		//printf("accept() is OK...\n\n");
		recv(new_fd, id, sizeof(id), 0);
		recv(new_fd, pw, sizeof(pw), 0);

		/* recive client's IP and PORT */
		
		 
		pid1 = fork();
		if(pid1 == 0) {
			if(!strcmp(id, "user1")) { 
				if(!strcmp(pw, "passwd1")) {
					new_fd1 = new_fd;
					printf(LOG_INF);					
					printf("\n>> ");
					send(new_fd1, LOG_INF, strlen(LOG_INF) + 1, 0);
					
					/* recive client1's IP and PORT */
					recv(new_fd1, msg, sizeof(msg), 0);
					strncpy(CLIENT1_IP,msg,sizeof(CLIENT1_IP));
					//printf("Client1 IP: %s\n", CLIENT1_IP);
					memset(msg, 0, 8);

					recv(new_fd1, msg, sizeof(msg), 0);
					strncpy(CLIENT1_PORT_S,msg,sizeof(CLIENT1_PORT_S));
					CLIENT1_PORT = atoi(CLIENT1_PORT_S);
					//printf("Client1 Port: %d\n", CLIENT1_PORT);
					memset(msg, 0, 8);
															
					pid2 = fork();
					memset(buf1, 0, 8);
					while(1) {
						if(pid2 == 0) {
							printf(">> ");
							recv(new_fd1, msg, sizeof(msg), 0);
							printf("[user1] %s\n", msg);
							if(!strcmp(msg, "[FILE]")){
								strcat(msg," ");
								strcat(msg,CLIENT1_IP);
								strcat(msg," ");
								strcat(msg,CLIENT1_PORT_S);
								write(fd2[1], msg, strlen(msg) + 1);
							}else{
							 	write(fd2[1], msg, strlen(msg) + 1);
							}							
						}
						else {
							read(fd1[0], buf1, sizeof(buf1));
							if(sizeof(buf1) != 0) {
								send(new_fd1, buf1, strlen(buf1) + 1, 0);
								memset(buf1, 0, 8);
							}
						}
					}
				}
			}
		}
		else {
			if(!strcmp(id, "user2")) {
				if(!strcmp(pw, "passwd2")) {
					new_fd2 = new_fd;
					send(new_fd2, LOG_INF, strlen(LOG_INF) + 1, 0);
					
					/* recive client2's IP and PORT */
					recv(new_fd2, msg, sizeof(msg), 0);
					strncpy(CLIENT2_IP,msg,sizeof(CLIENT2_IP));
					//printf("Client2 IP: %s\n", CLIENT2_IP);
					memset(msg, 0, 8);

					recv(new_fd2, msg, sizeof(msg), 0);
					strncpy(CLIENT2_PORT_S,msg,sizeof(CLIENT2_PORT_S));
					CLIENT2_PORT = atoi(CLIENT2_PORT_S);
					//printf("Client2 Port: %d\n", CLIENT2_PORT);
					memset(msg, 0, 8);

					pid3 = fork();
					memset(buf2, 0, 8);
					while(1) {
						if(pid3 == 0) {
							printf(">> ");
							recv(new_fd2, msg2, sizeof(msg2), 0);
							printf("[user2] %s\n", msg2);
							if(!strcmp(msg2, "[FILE]")){
								strcat(msg2," ");
								strcat(msg2,CLIENT2_IP);
								strcat(msg2," ");
								strcat(msg2,CLIENT2_PORT_S);
								write(fd1[1], msg2, strlen(msg2) + 1);
							}else{
							 	write(fd1[1], msg2, strlen(msg2) + 1);
							}
						}
						else {
							read(fd2[0], buf2, sizeof(buf2));
							if(sizeof(buf2) != 0) {
								send(new_fd2, buf2, strlen(buf2) + 1, 0);
								memset(buf2, 0, 8);
							}
						}
					}
				}
			}
		}
		}
		else
			close(sockfd);
	}
	return 0;
}
