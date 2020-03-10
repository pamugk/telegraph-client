#include "client.h"

struct User login(char* userId) {
    printf("Logging int.\n");
    enum ServerOperations operation = LOGIN;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    if (res == -1) {
        perror("send");
        exit(1);
    }
    res = send(sockfd, userId, 37 * sizeof(char), 0);
    if (res == -1) {
        perror("send");
        exit(1);
    }
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (res == -1) {
        perror("recv");
        exit(1);
    }
    struct User user;
    user.id = NULL;
    if (response == SUCCESS) {
        //res = recv(sockfd, &user, sizeof(struct User), 0);
        if (res == -1) {
            perror("recv");
            exit(1);
        }
        printf("%s %s\n", user.name, user.surname);
        printf("Done.\n");
    }
    else {
        printf("Failure\n");
    }
    return user;
}

int logout() {
    printf("Logging out.\n");
    enum ServerOperations operation = DISCONNECT;
    send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    close(sockfd);
    printf("Done.\n");
}