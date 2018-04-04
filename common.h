
/*
 * common.h
 * 
 * Jonathan Metzger
 * Spring 2018
 *
 * Project for CS4513 Distributed Computing Systems
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <crypt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

#define BUFSIZE 1024
#define USERCNT 5
#define PORT 4513

/* COMMON */

/* usage message */
void usage();
/* checks flag of input */
void flagCheck(int argc, char **argv);
/* concate two strings */
char* concat(char* s1, char* s2);
/* message contain terminate token */
int containToken(char* receiveMessageFromClient, int size);

/* ERROR HANDLERS */

/* sockets */
void ERROR_socket_call();
/* connect */
void ERROR_connect_call();
/* bind */
void ERROR_bind_call();
/* listen */
void ERROR_listen_call();
/* accept */
void ERROR_accept_call();
/* waitpid */
void ERROR_waitpid_call();
/* execvp */
void ERROR_execvp_call();

/* CLIENT */

/* get the host or address */
void getHost();
/* check the server */
void checkServer();
/* send message */
int sendMessageToServer(char* message, int sock);
/* receive message */
char* receiveMessageFromServer(int sock);
/* checks for valid username */
void checkUsername();
/* checks for valid password */
void checkPassword();
/* ask for username and checks if valid */
void checkCredentials();
/* receive commands from command line */
int submitInput(int sock);

/* SERVER */

/* counts commands from command line */
int countInputs(char* command);
/* stores commands from command line */
void storeInputs(char** command_array, char* command, int size_array);
/* set server to connect to */
void setServer();
/* open the server */
void openServer();
/* send message to client */
int sendMessageToClient(char* msg, int sock);
/* receive message from client */
char* receiveMessageFromClient(int sock);
/* get username from Client */
void getUsername();
/* checks username */
char* validUsername(char* name);
/* get password from Client */
void getPassword();
/* checks password */
char* validPassword(char* name);
/* get command from Client */
void getCommand();


