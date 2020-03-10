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

void groupDestructor(struct Group* group);

struct GroupList {
    int count;
    struct Group** list;
};

void groupListDestructor(struct GroupList* groupList);

struct Message {
    char* id;
    char* fromId;
    char* toId;
    char* text;
};

void messageDestructor(struct Message* message);

struct MessageList {
    int count;
    struct Message** list;
};

void messageListDestructor(struct MessageList* messageList);

struct User {
    char* id;
    char* phone;
    char* username;
    char* name;
    char* surname;
    char* biography;
};

void userDestructor(struct User* user);

struct UserList {
    int count;
    struct User** list;
};

void userListDestructor(struct UserList* userList);
#pragma endregion
#pragma endregion