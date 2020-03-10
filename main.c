#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

struct sockaddr_in setupServer() {
    FILE* settingsFile = fopen("settings.txt", "r");
    char* serverIp = "127.0.0.1";
    int serverPort = 8888;
    //fscanf(settingsFile, "%s%d", serverIp, &serverPort);
    fclose(settingsFile);
    struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIp);
	server.sin_port = htons(serverPort);
    return server;
}

int main() {
    struct sockaddr_in server = setupServer();
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket error");
		exit(1);
	}
	if (connect(sockfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("connect error");
		exit(1);
	}
    enum ServerOperations operation = LOGIN;
    send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    char * userId = "6f226870-1f26-4026-b244-c49bcc7dff12";
    send(sockfd, userId, 37 * sizeof(char), 0);
    enum ServerResponses response;
    recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        struct User* user;
        recv(sockfd, user, sizeof(struct User), 0);
        printf("%s %s\n", user->name, user->surname);
        free(user);
    }
    else {
        printf("Failure\n");
    }
    operation = DISCONNECT;
    send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    close(sockfd);
    return 0;
}