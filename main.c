#include "client.h"

struct sockaddr_in setupServer() {
    printf("Setting server up.\n");
    FILE* settingsFile = fopen("settings.txt", "r");
    if (settingsFile == NULL) {
        perror("fopen");
        exit(1);
    }
    char serverIp[16];
    int serverPort;
    int res = fscanf(settingsFile, "%s%d", serverIp, &serverPort);
    if (res == -1) {
        perror("fscanf");
        fclose(settingsFile);
        exit(1);
    }
    fclose(settingsFile);
    struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIp);
	server.sin_port = htons(serverPort);
    printf("Done.\n");
    return server;
}

int main() {
    struct sockaddr_in server = setupServer();
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket error");
		exit(1);
	}
	if (connect(sockfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("connect error");
		exit(1);
	}
    login("a2319689-c3c6-4b1f-b509-bdc314974c32");
    logout();
    return 0;
}