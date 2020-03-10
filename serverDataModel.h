enum ServerOperations {
    ADD_CONTACT,
    ADD_USER_TO_GROUP,
    CLEAR_HISTORY,
    CREATE_GROUP,
    DISCONNECT,
    GET_CONTACTS,
    GET_GROUP_INFO,
    GET_MESSAGES,
    GET_USER,
    GET_USER_GROUPS,
    LOGIN,
    REGISTER_USER,
    REMOVE_GROUP,
    REMOVE_MESSAGE,
    REMOVE_USER,
    RESEND_MESSAGE,
    SEND_MESSAGE
};

enum ServerResponses {
    SUCCESS,
    FAILURE
};

#pragma region Data model
struct Group {
    char* id;
    char* creatorId;
    int countOfParticipants;
    char** participants;
    char* name;
};

void groupDestructor(struct Group* group) {
    free(group->id);
    free(group->creatorId);
    for (int i = 0; i < group->countOfParticipants; i += 1)
        free(group->participants[i]);
    free(group->participants);
}

struct GroupList {
    int count;
    struct Group* list;
};

void groupListDestructor(struct GroupList* groupList) {
    for (int i = 0; i < groupList->count; i += 1)
        groupDestructor(groupList->list + i);
    free(groupList->list);
}

struct Message {
    char* id;
    char* fromId;
    char* toId;
    char* text;
};

void messageDestructor(struct Message* message) {
    free(message->id);
    free(message->toId);
    free(message->fromId);
    free(message->text);
}

struct MessageList {
    int count;
    struct Message* list;
};

void groupListDestructor(struct MessageList* messageList) {
    for (int i = 0; i < messageList->count; i += 1)
        messageDestructor(messageList->list + i);
    free(messageList->list);
}

struct User {
    char* id;
    char* phone;
    char* username;
    char* name;
    char* surname;
    char* biography;
};

void userDestructor(struct User* user) {
    free(user->id);
    free(user->phone);
    free(user->username);
    free(user->name);
    free(user->surname);
    free(user->biography);
}

struct UserList {
    int count;
    struct User* list;
};

void groupListDestructor(struct UserList* userList) {
    for (int i = 0; i < userList->count; i += 1)
        userDestructor(userList->list + i);
    free(userList->list);
}
#pragma endregion