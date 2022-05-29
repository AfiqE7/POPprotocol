#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

struct clientData{
	int idsocket;
	char email[100];
	int clients[100];
}

totalClients = 0;
void* handleClient(void*);
int listenClient(struct clientData);

int main(){

	char *ip = "192.168.56.101";
	int port = 110;
  
	int server_sock, client_sock;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_size;
	char buffer[1024];
	int n;
 	
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0){
		perror("[-]Socket error");
		exit(1);
	}
	
	printf("[+]TCP server socket created.\n");
	
	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	
	n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(n < 0){
		perror("[-]Bind error");
		exit(1);
	}

	printf("[+]Bind to the port number: %d\n", port);

	listen(server_sock, 5);
	printf("Listening ...\");

	while(1){
		bzero(buffer, 1024);
		addr_size = sizeof(client_addr);
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr , &addr_size);
		printf("[+]Client %d connected.\n", client_sock);
		printf("[+]Client setting up email...\n");
		recv(client_sock, buffer, sizeof(buffer), 0);
		int index = totalClients++;
		clients[index].idsocket = client_sock;
		strcpy(clients[index].email, buffer);
		printf("[+]Client email established as %s...\n", clients[index].email);
		pthread_t thread;
		pthread_create(&thread, NULL, handleClient, &clients[index]);
		printf("[+]Ready to connect to another client\n");
	}
	
	return 0;

int listenClient(struct clientData client){
	char buffer[1024];
	printf("Listening to %d\n", client.idsocket);
	ssize_t value = recv(client.socket_id, buffer, sizeof(buffer), 0);
	
	if(value == 0){
		printf("Client id shutdown.\n", client.idsocket);
		return 0;
	}

	else if (value == -1){
		printf("Error on listening to client %d\n", client.idsocket);
		return 0;
	}

	else{
		if (strlen(buffer) > 0){
			char copied[1024];
			strcpy(copied, buffer);
			char* token = strtok(buffer, "|");
			printf("Receiving from %s, sending to %s/n", client.email, token);
			
			for(int i = 0; i < 100; i++){
				struct clientData currentClient = clients[i];
				if (strcmp(currentClient.email, token) == 0){
					send(currentClient.socket_id, copied, strlen(copied), 0);
					printf(
						"Successful sending to socket %d with email %s\n", currentClient.idsocket, currentClient.emails);
					return1;
				}
			}
			printf("Fail to send the email %s/n", token); 
		}
		return 1;
	}
}

void* handleClient(void* value){
	struct clientData* clientd = (struct clientData*) value;
	struct clientData client = *clientd;
	printf("[+]Listening to client %d...\n", client.idsocket);
  }

}
