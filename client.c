#include "client.h"
#pragma region Auxillary functions
char* recieveStr(int sockfd) {
    int size = 0;
    recv(sockfd, &size, sizeof(int), 0);
    char* str = calloc(size, sizeof(char));
    recv(sockfd, str, size * sizeof(char), 0);
    return str;
}

struct User* recieveUser(int sockfd) {
    struct User* user = (struct User*) malloc(sizeof(struct User));
    user->id = recieveStr(sockfd);
    user->phone = recieveStr(sockfd);
    user->username = recieveStr(sockfd);
    user->name = recieveStr(sockfd);
    user->surname = recieveStr(sockfd);
    user->biography = recieveStr(sockfd);
    return user;
}
#pragma endregion
#pragma region Client functions
struct User* login(char* userId) {
    printf("Logging int.\n");
    enum ServerOperations operation = LOGIN;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = send(sockfd, userId, 37 * sizeof(char), 0);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    struct User* user;
    user->id = NULL;
    if (response == SUCCESS) {
        user = recieveUser(sockfd);
        printf("%s %s\n", user->name, user->surname);
        printf("Done.\n");
    }
    else
        printf("Failure\n");
    return user;
}

int logout() {
    printf("Logging out.\n");
    enum ServerOperations operation = DISCONNECT;
    send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    close(sockfd);
    printf("Done.\n");
}
#pragma endregion