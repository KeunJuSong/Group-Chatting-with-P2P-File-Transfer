#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>	// for use mkdir & chdir  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>	// for use file list function

#define SERV_IP "220.149.128.100"
#define SERV_PORT 4130

#define CLIENT1_IP "220.149.128.101"
#define CLIENT1_PORT "4131"
#define CLIENT1_PORT_R 4131

int main(int argc, char *argv[])
{
	int sockfd, new_fd = 0;
	int sockfd1, sockfd2 = 0;
	struct sockaddr_in dest_addr;

	int rcv_byte;
	char buf[512];

	char id[20];
	char pw[20];
	char msg[512];
	char msg1[512];
	char fd[20];
	
	char CLIENT2_IP_C[15];
	char CLIENT2_IP[16];
	char CLIENT2_PORTBUFF[5];
	char *ptr;
	int CLIENT2_PORT;

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

	if(!strcmp(id, "user1")) 		//Comparing recieved id whether user1 or not
		if(!strcmp(pw, "passwd1")){	//Comparing recieved pw whether passwd1 or not
			recv(sockfd, msg, sizeof(msg), 0);
			printf("%s\n", msg);
			memset(msg, 0, 8);
			
			/* send client1's IP and PORT */
			send(sockfd, CLIENT1_IP, strlen(CLIENT1_IP) + 1, 0);
			
			send(sockfd, CLIENT1_PORT, strlen(CLIENT1_PORT) + 1, 0);
			
		    	pid_t pid;
			pid_t pid1;
			pid_t pid2;
			pid_t pid3;
			pid_t pid4;
			pid = fork();
			while(1) {
				if(pid == 0)  {
					printf(">> ");
					scanf("%s", msg);
					send(sockfd, msg, strlen(msg) + 1, 0);
					if(!strcmp(msg, "exit")) {
						exit(1);
					}
					if(!strcmp(msg,"[FILE]")) {
						printf("%s\n", "FILE request!");
						
						pid1 = fork();
						
						/* fork - P2P server */
						if(pid1 == 0) {
							close(sockfd);
	
							struct sockaddr_in my_addr;
							struct sockaddr_in their1_addr;
							unsigned int sin_size;

							int val = 1;
							int rcv_byte1;
							char msg_file[512];
							char msg_request[20];
							char filename_server[8];
							char file_contents[20];
							FILE *src_server;
				
							int count_server = 1;
							DIR *dir_info_server;
							struct dirent *dir_entry_server;
							
							sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
							
							if(sockfd1 == 1) {
								perror("Server-socket() error lol!");
								exit(1);
							}
							else printf("Server-socket() sockfd1 is OK...\n");
	
							my_addr.sin_family = AF_INET;
							my_addr.sin_port = htons(CLIENT1_PORT_R);
							my_addr.sin_addr.s_addr = INADDR_ANY;
							memset(&(my_addr.sin_zero), 0, 8);

							if(setsockopt(sockfd1, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val)) < 0) {
								perror("setsockopt");
								close(sockfd1);
								return -1;
							}

							if(bind(sockfd1, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
								perror("Server-blind() error lol!");
								exit(1);
							}
							else printf("Server-blind() is OK...\n");

							if(listen(sockfd1, 10) == -1) {
								perror("listen() error lol!");
								exit(1);
							}
							else printf("listen() is OK...\n\n");

							sin_size = sizeof(struct sockaddr_in);
							
							new_fd = accept(sockfd1, (struct sockaddr *)&their1_addr, &sin_size);
							if(new_fd == -1) perror("accept() error lol!");
							else printf("accept() is OK...\n\n");
							
							/* recv the file list */
							printf("-------------FILE LIST-----------------\n");
							recv(new_fd, msg_file, sizeof(msg_file), 0);
							printf("%s\n\n",msg_file);
							
							/* request the file number of list */
							printf("choose the file number!(Only for .txt file!!): ");
							scanf("%s",msg_request);
							send(new_fd, msg_request, strlen(msg_request) + 1, 0);

							/* recieve the request file name & file contents*/
							recv(new_fd, msg_file, sizeof(msg_file), 0);
							strncpy(filename_server,msg_file,sizeof(filename_server));
							printf("file name: %s\n",filename_server);
							
							recv(new_fd, msg_file, sizeof(msg_file), 0);
							strncpy(file_contents,msg_file,sizeof(file_contents));
							printf("file contents: %s\n",file_contents);
								
							/* make the file */
							src_server = fopen(filename_server,"w");
							fprintf(src_server,"%s\n",file_contents);
							fclose(src_server);
							
							/* confirm the new file */
							printf("show the requested file!\n");
							dir_info_server = opendir( ".");              // 현재 디렉토리를 열기
   							if ( NULL != dir_info_server)
   							{
      								while( dir_entry_server   = readdir( dir_info_server))  // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
      								{
         								printf( "%d.%s\n", count_server,dir_entry_server->d_name);
									count_server++;
											
      								}
      								closedir(dir_info_server);
   							}	
							
							printf("P2P File Transfer Server Terminate!\n");
							
							close(sockfd1);
							close(new_fd);
							exit(1);
						}
						else{
							close(sockfd);
							exit(1);
						}
					}
				}						
				else {
					if(rcv_byte = recv(sockfd, msg1, sizeof(msg1), 0) >= 0) {
						printf("[user2] %s\n>> ", msg1);
						if(strlen(msg1) == 27){

							/* stirng message seperate for client2 IP & Port */
							ptr = strstr(msg1, " ");
							strncpy(CLIENT2_IP_C,ptr+1,sizeof(CLIENT2_IP_C));
							//printf("client2_ip_c length:%d\n",strlen(CLIENT2_IP_C));
							//printf("CLIENT2_IP_C:%s\n",CLIENT2_IP_C);
							strncpy(CLIENT2_IP,CLIENT2_IP_C,sizeof(CLIENT2_IP));
							//printf("CLIENT2_IP:%s\n",CLIENT2_IP);
							strncpy(CLIENT2_IP,CLIENT2_IP,sizeof(CLIENT2_IP));
							//printf("CLIENT2_IP1:%s\n",CLIENT2_IP);
							ptr = strstr(ptr+1, " ");
							strncpy(CLIENT2_PORTBUFF,ptr,sizeof(CLIENT2_PORTBUFF));
							CLIENT2_PORT = atoi(CLIENT2_PORTBUFF);
							//printf("CLIENT2_PORT:%d\n",CLIENT2_PORT);
							
							/* fork() - P2P client */
							pid2 = fork();
							
							if(pid2 == 0) {
									close(sockfd);
											
									struct sockaddr_in dest1_addr;
									char msg_file_recv[512];
									char filename[10];
									char newfileforsend[100];
									char newfileforsend2[] = { };
									char buffer[20];
									int count = 1;
									int file_number;
									FILE *src;
									DIR *dir_info;
									struct dirent *dir_entry;	

									sockfd2 = socket(AF_INET, SOCK_STREAM, 0);

									if(sockfd2 == -1) {
									perror("Client-socket() error lol!");
									exit(1);
									}
									else printf("Client-socket() is OK...\n");

									dest1_addr.sin_family = AF_INET;

									dest1_addr.sin_port = htons(CLIENT2_PORT);
										
									dest1_addr.sin_addr.s_addr = inet_addr(CLIENT2_IP);

									memset(&(dest1_addr.sin_zero), 0, 8);

									if(connect(sockfd2, (struct sockaddr *)&dest1_addr, sizeof(struct sockaddr)) == -1) {
										perror("Client-connect() error lol!");
										exit(1);
									}
									else printf("Client-connect() is OK...\n\n");
									
									/* make "FILE" directory */
									mkdir("FILE",0755);
									if(chdir("FILE") != -1){
										/* 4 .txt files create */
										for(int i = 0; i < 4; i++) {
											sprintf(filename, "a-%d.txt",i);
											src = fopen(filename,"w");
											fprintf(src,"%d_contents!\n",i);
											fclose(src);
										}
									}

									
									printf("---------------FILE LIST--------------\n");
									/* show file list */
									dir_info = opendir(".");              // 현재 디렉토리를 열기
   									if ( NULL != dir_info)
   									{
      										while( dir_entry   = readdir( dir_info))  // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
      										{
         										printf( "%d.%s\n", count,dir_entry->d_name);
											sprintf(newfileforsend,"%d.%s\n", count,dir_entry->d_name);
											strcat(newfileforsend2,newfileforsend);
											count++;
											
      										}
      										closedir(dir_info);
   									}	
									send(sockfd2, newfileforsend2, strlen(newfileforsend2) + 1, 0);
									
									/* receive the file number */
									recv(sockfd2, msg_file_recv, sizeof(msg_file_recv), 0);
									printf("Requested the file number!: %s\n",msg_file_recv);
									
									/* Transfer the File */
									file_number = atoi(msg_file_recv); 
									switch(file_number){
										case 2:
											send(sockfd2, "a-2.txt", strlen("a-2.txt")+1,0);
											src = fopen("a-2.txt","r");
											fgets(buffer,sizeof(buffer),src);
											send(sockfd2, buffer, strlen(buffer)+1,0);
											printf("============'a-2.txt' file transfer......!============\n\n");
											break;
										case 3:
											send(sockfd2, "a-3.txt", strlen("a-3.txt")+1,0);
											src = fopen("a-3.txt","r");
											fgets(buffer,sizeof(buffer),src);
											send(sockfd2, buffer, strlen(buffer)+1,0);
											printf("============'a-3.txt' file transfer......!============\n\n");
											break;
										case 5:
											send(sockfd2, "a-1.txt", strlen("a-1.txt")+1,0);
											src = fopen("a-1.txt","r");
											fgets(buffer,sizeof(buffer),src);
											send(sockfd2, buffer, strlen(buffer)+1,0);
											printf("============'a-1.txt' file transfer......!============\n\n");
											break;
										case 6:
											send(sockfd2, "a-0.txt", strlen("a-0.txt")+1,0);
											src = fopen("a-0.txt","r");
											fgets(buffer,sizeof(buffer),src);
											send(sockfd2, buffer, strlen(buffer)+1,0);
											printf("============'a-0.txt' file transfer......!============\n\n");
											break;
										default:
											printf("wrong request file number!!(Only For .txt file!!)\n");
									}
									printf("P2P - File Transfer Client Terminate!\n");
									close(sockfd2);
									exit(1);
								}
								else {
									close(sockfd);
									exit(1);
								}
						}else{
							memset(msg1, 0, 8);
						}
					}
				}
			}	
		}
		else exit(1);

	return 0;
}
