
/*
 * common.c
 * 
 * Jonathan Metzger
 * Spring 2018
 *
 * Project for CS4513 Distributed Computing Systems
 * Source: https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
 *
 */

#include "common.h"

/* client send username to server (not password) */

/* server responds by sending back unique random number */

/* Client encrypts using user’s password plus number as key */

/* Client sends hashed/encrypted value back to server */

/* Server encrypts using the user’s same password plus number as key */

/* Server compares two hashed/encrypted values, if same then ok */

int sock;
int i;
int option;
struct sockaddr_in serv;
char* result = NULL;
int waitpid_call;
int execvp_call;
int status;
int accept_client;

/* checks to see if terminate token is present */
int containToken(char* receive_message, int size)
{
	for(i = 0; i < size; i++)
	{
		if(receive_message[i] == 4)
		{
			return i;
		}
	}
	return -1;
}

/* concate two strings */
char* concat(char* s1, char* s2)
{
    result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


/* ERRORS HANDLERS */


/* sockets */
void ERROR_socket_call()
{
	option = 1;
	/* get socket */
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket()");
		exit(-1);
	}
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
}

/* connect */
void ERROR_connect_call()
{
	if(connect(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0)
	{
		perror("connect()");
		exit(-1);
	}
}

/* bind */
void ERROR_bind_call(struct sockaddr_in server_address)
{
	if(bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		perror("bind()");
		exit(-1);
	}
}

/* listen */
void ERROR_listen_call()
{
	if(listen(sock, 5))
	{
		perror("listen()");
		exit(-1);
	}
}

/* accept */
void ERROR_accept_call()
{
	if(accept_client < 0)
	{
		perror("accept()");
		exit(-1);
	}
}
/* waitpid */
void ERROR_waitpid_call()
{
	while(waitpid_call > 0)
	{
		waitpid_call = waitpid(-1, &status, WNOHANG);
	}
	if(waitpid_call < 0)
	{
		perror("waitpid()");
		exit(-1);
	}
}

/* execvp */
void ERROR_execvp_call()
{
	if(execvp_call)
	{
		perror("execvp()");
		exit(-1);
	}
}
