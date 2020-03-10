#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include "serverDataModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int sockfd;

struct User* login(char* userId);
int logout();
char* registerUser(struct User*);
int removeGroup(char*);
int removeMessage(struct Message*);
int removeUser(char*);
int resendMessage(struct Message*);
char* sendMessage(struct Message*);