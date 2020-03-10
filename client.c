#include "client.h"

#pragma region Destructors
void groupDestructor(struct Group* group) {
    free(group->id);
    free(group->creatorId);
    for (int i = 0; i < group->countOfParticipants; i += 1)
        free(group->participants[i]);
    free(group->participants);
    free(group);
}

void groupListDestructor(struct GroupList* groupList) {
    for (int i = 0; i < groupList->count; i += 1)
        groupDestructor(groupList->list[i]);
    free(groupList->list);
    free(groupList);
}

void messageDestructor(struct Message* message) {
    free(message->id);
    free(message->toId);
    free(message->fromId);
    free(message->text);
    free(message);
}

void messageListDestructor(struct MessageList* messageList) {
    for (int i = 0; i < messageList->count; i += 1)
        messageDestructor(messageList->list[i]);
    free(messageList->list);
    free(messageList);
}

void userDestructor(struct User* user) {
    free(user->id);
    free(user->phone);
    free(user->username);
    free(user->name);
    free(user->surname);
    if (user->biography != NULL)
        free(user->biography);
    free(user);
}

void userListDestructor(struct UserList* userList) {
    for (int i = 0; i < userList->count; i += 1)
        userDestructor(userList->list[i]);
    free(userList->list);
    free(userList);
}
#pragma endregion
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