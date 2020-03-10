#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include "serverDataModel.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int sockfd;

struct User login(char* userId);
int logout();