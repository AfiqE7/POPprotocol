#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void connectToServer(int, struct sockaddr_in);
int sendEmail(int);
void handleServ(int);

int main(){
	char *ip = "192.168.56.101";
	
	int port = 110;
	int sock;
	struct sockaddr_in addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock < 0){
		perror("[-]Socket error");
		exit(1);
	}
	
	printf("[+]TCP server socket created.\n");
	memset(&addr, '\0', sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = inet_addr(ip);
	connectToServer(sock, addr);
	
	if (sendEmail(sock) < 0){
		close(sock);
		return -1;
	}
	
	handleServ(sock);
	close(sock);
	printf("Disconnected from the server.\n");
	return 0;

}

void handleSend(int sock){
	printf("Enter contact email: ");
	char emailToSend[1024];
	memset(emailToSend, 0, 1024);
	char emailAdd[100];
	memset(emailAdd, 0, 100);
	fgets(emailAdd, 100, stdin);
	printf("Subject: ");
	char emailHead[100];
	memset(emailHead, 0, 100);
	fgets(emailHead, 100, stdin);
	printf("Description: ");
	char emailDesc[840];
	memset(emailDesc, 0, 840);
	fgets(emailDesc, 100, stdin);
	
	char discrim[] = "1";
	strcat(emailAdd, discrim);
	strcat(emailToSend, emailAdd);
	strcat(emailHead, discrim);
	strcat(emailToSend, emailHead);
	strcat(emailToSend, emailDesc);
	send(sock, emailToSend, strlen(emailToSend), 0);
	printf("Sending email to %s", emailAdd);
}
	
void handleReceive(int sock){
	char buffer[1024];
	memset(buffer, 0, 1024);
	recv(sock, buffer, sizeof(buffer), 0);
	if (strlen(buffer) != 0 ){
		char* emailFrom = strtok(buffer, "|");
		char* emailHeader = strtok(NULL, "|");
		char* emailDesc = strtok(NULL, "|");
		printf(
			"\nReceiving from %s\nSubject: %s\nDescription:\n%s\n", emailFrom, emailHeader, emailDesc);
		}
}

void handleServer(int sock){
	int id = fork();
	if (id == 0){
		while(1)
			handleSend(sock);
	}
	else{
		while(1);
		handleReceive(sock);
	}
}
	
void connectToServer(int sock, struct sockaddr_in addr){
	int value = connect(sock,(struct sockaddr*)&addr, sizeof(addr));
	if (value < 0){
		printf("Connection to the server failure. Exiting ...");
		exit(1);
	}
	printf("Connected to the server.\n");
}

}
