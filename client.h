#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include "serverDataModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int sockfd;

int addContact(char*, char*);
int addUserToGroup(char*, char*);
int clearHistory(char*, char*);
char* createGroup(struct Group*);
struct UserList* getContacts(char*);
struct Group* getGroupInfo(char*);
struct MessageList* getMessages(char*, char*);
struct User* getUser(char*);
struct GroupList* getUserGroups(char*);
struct User* login(char*);
int logout();
char* registerUser(struct User*);
int removeGroup(char*);
int removeMessage(struct Message*);
int removeUser(char*);
int resendMessage(struct Message*);
char* sendMessage(struct Message*);