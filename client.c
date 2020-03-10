#include "client.h"

#pragma region Destructors
void groupDestructor(struct Group* group) {
    if (group->id != NULL)
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
    if (message->id != NULL)
        free(message->id);
    free(message->toId);
    free(message->fromId);
    if (message->text != NULL)
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
    if (user->id != NULL)
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
char* doRecieveStr(int sockfd) {
    int size = 0;
    recv(sockfd, &size, sizeof(int), 0);
    char* str = calloc(size, sizeof(char));
    recv(sockfd, str, size * sizeof(char), 0);
    return str;
}

struct Group* doRecieveGroup(int nsock) {
    struct Group* group = (struct Group*) malloc(sizeof(struct Group));
	group->id = doRecieveStr(nsock);
	group->creatorId = doRecieveStr(nsock);
	group->name = doRecieveStr(nsock);
    recv(nsock, &group->countOfParticipants, sizeof(int), 0);
    for (int i = 0; i < group->countOfParticipants; i += 1)
        group->participants[i] = doRecieveStr(nsock);
	return group;
}

struct GroupList* doRecieveGroups(int nsock) {
    struct GroupList* groups = (struct GroupList*) malloc(sizeof(struct GroupList));
    recv(nsock, &groups->count, sizeof(int), 0);
    groups->list = (struct Group**)calloc(groups->count, sizeof(struct Group*));
    for (int i = 0; i < groups->count; i += 1)
        groups->list[i] = doRecieveGroup(nsock);
	return groups;
}

struct Message* doRecieveMessage(int nsock, int recieveHeaderOnly) {
	struct Message* message = (struct Message*) malloc(sizeof(struct Message));
	message->id = doRecieveStr(nsock);
	message->fromId = doRecieveStr(nsock);
	message->toId = doRecieveStr(nsock);
    if (recieveHeaderOnly == 0)
	    message->text = doRecieveStr(nsock);
	return message;
}

struct MessageList* doRecieveMessages(int nsock) {
    struct MessageList* messages = (struct MessageList*) malloc(sizeof(struct MessageList));
    recv(nsock, &messages->count, sizeof(int), 0);
    messages->list = (struct Message**)calloc(messages->count, sizeof(struct Message*));
    for (int i = 0; i < messages->count; i += 1)
        messages->list[i] = doRecieveMessage(nsock, 0);
	return messages;
}

struct User* doRecieveUser(int sockfd) {
    struct User* user = (struct User*) malloc(sizeof(struct User));
    user->id = doRecieveStr(sockfd);
    user->phone = doRecieveStr(sockfd);
    user->username = doRecieveStr(sockfd);
    user->name = doRecieveStr(sockfd);
    user->surname = doRecieveStr(sockfd);
    user->biography = doRecieveStr(sockfd);
    return user;
}

struct UserList* doRecieveUsers(int nsock) {
    struct UserList* users = (struct UserList*) malloc(sizeof(struct UserList));
    recv(nsock, &users->count, sizeof(int), 0);
    users->list = (struct User**)calloc(users->count, sizeof(struct User*));
    for (int i = 0; i < users->count; i += 1)
        users->list[i] = doRecieveUser(nsock);
	return users;
}

int doSendStr(int nsock, const char* str) {
	int size = strlen(str);
	send(nsock, &size, sizeof(int), 0);
	send(nsock, str, size * sizeof(char), 0);
	return 0;
}

int doSendGroup(int nsock, struct Group* group) {
	doSendStr(nsock, group->id);
	doSendStr(nsock, group->creatorId);
	doSendStr(nsock, group->name);
    send(nsock, &group->countOfParticipants, sizeof(int), 0);
    for (int i = 0; i < group->countOfParticipants; i += 1)
        doSendStr(nsock, group->participants[i]);
	return 0;
}

int doSendMessage(int nsock, struct Message* message, int sendHeaderOnly) {
	doSendStr(nsock, message->id);
	doSendStr(nsock, message->fromId);
	doSendStr(nsock, message->toId);
    if (sendHeaderOnly == 0)
	    doSendStr(nsock, message->text);
	return 0;
}

int doSendUser(int nsock, struct User* user) {
	doSendStr(nsock, user->id);
	doSendStr(nsock, user->phone);
	doSendStr(nsock, user->username);
	doSendStr(nsock, user->name);
	doSendStr(nsock, user->surname);
	doSendStr(nsock, user->biography);
	return 0;
}
#pragma endregion
#pragma region Client functions
int addContact(char* userId, char* contactId) {
    printf("Adding contact.\n");
    enum ServerOperations operation = ADD_CONTACT;
    send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendStr(sockfd, userId);
    doSendStr(sockfd, contactId);
    enum ServerResponses response;
    int res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        printf("Done.\n");
        return 0;
    }
    printf("Failure\n");
    return 1;
}

int addUserToGroup(char* groupId, char* userId) {
    printf("Adding user to group.\n");
    enum ServerOperations operation = ADD_USER_TO_GROUP;
    send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendStr(sockfd, groupId);
    doSendStr(sockfd, userId);
    enum ServerResponses response;
    int res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        printf("Done.\n");
        return 0;
    }
    printf("Failure\n");
    return 1;
}

int clearHistory(char* fromId, char* toId) {
    printf("Clearing history of messages.\n");
    enum ServerOperations operation = ADD_USER_TO_GROUP;
    send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendStr(sockfd, fromId);
    doSendStr(sockfd, toId);
    enum ServerResponses response;
    int res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        printf("Done.\n");
        return 0;
    }
    printf("Failure\n");
    return 1;
}

char* createGroup(struct Group* newGroup) {
    printf("Asking for the creation of a new group.\n");
    enum ServerOperations operation = CREATE_GROUP;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendGroup(sockfd, newGroup);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    char* createdGroupId = NULL;
    if (response == SUCCESS) {
        createdGroupId = doRecieveStr(sockfd);
        printf("Done.\n");
    }
    else
        printf("Failure.\n");
    return createdGroupId;
}

struct UserList* getContacts(char* userId) {
    printf("Asking for a contacts.\n");
    enum ServerOperations operation = GET_CONTACTS;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, userId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    struct UserList* contacts = NULL;
    if (response == SUCCESS) {
        contacts = doRecieveUsers(sockfd);
        printf("Done.\n");
    }
    else
        printf("Failure\n");
    return contacts;
}

struct Group* getGroupInfo(char* groupId) {
    printf("Fetching group.\n");
    enum ServerOperations operation = GET_GROUP_INFO;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, groupId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    struct Group* group = NULL;
    if (response == SUCCESS) {
        group = doRecieveGroup(sockfd);
        printf("Done.\n");
    }
    else
        printf("Failure\n");
    return group;
}

struct MessageList* getMessages(char* fromId, char* toId) {
    printf("Fetching messages.\n");
    enum ServerOperations operation = GET_MESSAGES;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, fromId);
    res = doSendStr(sockfd, toId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    struct MessageList* messages = NULL;
    if (response == SUCCESS) {
        messages = doRecieveMessages(sockfd);
        printf("Done.\n");
    }
    else
        printf("Failure\n");
    return messages;
}

struct User* getUser(char* userId) {
    printf("Fetching user.\n");
    enum ServerOperations operation = GET_USER;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, userId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    struct User* user = NULL;
    if (response == SUCCESS) {
        user = doRecieveUser(sockfd);
        printf("Done.\n");
    }
    else
        printf("Failure\n");
    return user;
}

struct GroupList* getUserGroups(char* userId) {
    printf("Asking for a groups.\n");
    enum ServerOperations operation = GET_USER_GROUPS;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, userId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    struct GroupList* groups = NULL;
    if (response == SUCCESS) {
        groups = doRecieveGroups(sockfd);
        printf("Done.\n");
    }
    else
        printf("Failure\n");
    return groups;
}

struct User* login(char* userId) {
    printf("Logging in.\n");
    enum ServerOperations operation = LOGIN;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, userId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    struct User* user = NULL;
    if (response == SUCCESS) {
        user = doRecieveUser(sockfd);
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

char* registerUser(struct User* user) {
    printf("Asking for the registration of a new user.\n");
    enum ServerOperations operation = REGISTER_USER;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendUser(sockfd, user);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    char* registeredUserId = NULL;
    if (response == SUCCESS) {
        registeredUserId = doRecieveStr(sockfd);
        printf("Done.\n");
    }
    else
        printf("Failure.\n");
    return registeredUserId;
}

int removeGroup(char* groupId) {
    printf("Asking for the removal of a group.\n");
    enum ServerOperations operation = REMOVE_GROUP;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, groupId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        printf("Done.\n");
        return 0;
    }
    printf("Failure.\n");
    return 1;
}

int removeMessage(struct Message* message) {
    printf("Asking for the removal of a message.\n");
    enum ServerOperations operation = REMOVE_MESSAGE;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendMessage(sockfd, message, 1);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        printf("Done.\n");
        return 0;
    }
    printf("Failure.\n");
    return 1;
}

int removeUser(char* userId) {
    printf("Asking for the removal of a user.\n");
    enum ServerOperations operation = REMOVE_USER;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    res = doSendStr(sockfd, userId);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        printf("Done.\n");
        return 0;
    }
    printf("Failure.\n");
    return 1;
}

int resendMessage(struct Message* message) {
    printf("Asking for the resending of a message.\n");
    enum ServerOperations operation = RESEND_MESSAGE;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendMessage(sockfd, message, 1);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        printf("Done.\n");
        return 0;
    }
    printf("Failure.\n");
    return 1;
}

char* sendMessage(struct Message* newMessage) {
    printf("Asking for the sending of a message.\n");
    enum ServerOperations operation = SEND_MESSAGE;
    int res = send(sockfd, &operation, sizeof(enum ServerOperations), 0);
    doSendMessage(sockfd, newMessage, 0);
    enum ServerResponses response;
    res = recv(sockfd, &response, sizeof(enum ServerResponses), 0);
    if (response == SUCCESS) {
        char* messageId = doRecieveStr(sockfd);
        printf("Done.\n");
        return messageId;
    }
    printf("Failure.\n");
    return NULL;
}
#pragma endregion