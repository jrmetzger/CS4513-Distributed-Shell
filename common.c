
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

int sock;
struct sockaddr_in serv;

/* checks to see if terminate token is present */
int containToken(char* receive_message, int size)
{
	for(int i=0; i<size;i++)
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
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


/* ERRORS HANDLERS */


/* sockets */
void ERROR_socket_call()
{
	int option = 1;
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
