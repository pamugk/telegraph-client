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
    mainSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mainSocket == -1) {
		perror("socket error");
		exit(1);
	}
    notifierSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (notifierSocket == -1) {
		perror("socket error");
		exit(1);
	}
	if (connect(mainSocket, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("connect error");
		exit(1);
	}
	if (connect(notifierSocket, (struct sockaddr *) &server, sizeof(server)) == -1) {
		perror("connect error");
        close(mainSocket);
		exit(1);
	}
    int mode;
    scanf("%d", &mode);
    switch (mode)
    {
        case 1: {
            struct User* user = login("a2319689-c3c6-4b1f-b509-bdc314974c32");
            struct Message* message = (struct Message*)malloc(sizeof(struct Message));
            message->fromId = "a2319689-c3c6-4b1f-b509-bdc314974c32";
            message->toId = "22dcee36-4882-4406-b9b6-8ca440cfed7a";
            message->text = "QQ";
            message->id = sendMessage(message);
            int c;
            wait(&c);
            if (user != NULL)
                userDestructor(user);
            if (message != NULL)
                messageDestructor(message);
            break;
        }
        case 2: {
            struct User* user = login("22dcee36-4882-4406-b9b6-8ca440cfed7a");
            printf("Waiting for messages...\n");
            int c;
            wait(&c);
            break;
        }
    }
    return 0;
}