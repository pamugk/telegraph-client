#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include "serverDataModel.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

extern int mainSocket;
extern int notifierSocket;

int addContact(char*);
int addUserToGroup(char*, char*);
int clearHistory(char*);
char* createGroup(struct Group*);
struct UserList* getContacts();
struct Group* getGroupInfo(char*);
struct MessageList* getMessages(char*);
struct User* getUser(char*);
struct GroupList* getUserGroups(char*);
struct User* login(char*);
int logout();
char* registerUser(struct User*);
int removeGroup(char*);
int removeMessage(struct Message*);
int removeUser();
int resendMessage(struct Message*);
char* sendMessage(struct Message*);