
/*
 * server.c
 * 
 * Jonathan Metzger
 * Spring 2018
 *
 * Project for CS4513 Distributed Computing Systems
 *
 */

/* client send username to server (not password) */

/* server responds by sending back unique random number */

/* Client encrypts using user’s password plus number as key */

/* Client sends hashed/encrypted value back to server */

/* Server encrypts using the user’s same password plus number as key */

/* Server compares two hashed/encrypted values, if same then ok */


#include "common.h"
#include "common.c"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/wait.h>

#include <crypt.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

/* set credentials */
char* valid_username[USERCNT] = {"user1", "user2", "user3", "user4", "user5"};
char* valid_password[USERCNT] = {"pass1", "pass2", "pass3", "pass4", "pass5"};
char* clientName = NULL;


time_t t;

struct sockaddr_in client_address;
struct sockaddr_in server_address;

int i;
int bytes;
int option;
int pid;
int sock;
int port=4513;
int status;

int client_length;
int accept_client;
int waitpid_call;


/* main */
int main(int argc, char** argv)
{
	/* flag input */
	flagCheck(argc, argv);

	/* set the server */
	setServer();

	/* open the server */
	openServer();
}

/* TODO */

/* open the server */
void openServer()
{
	//printf("Opening Server: %s with Port: %d!\n", server, port);
	while(1)
	{
		// Wait for client connection.
		client_length = sizeof(client_address);
		
		accept_client = accept(sock, (struct sockaddr *)&client_address, &client_length);
		ERROR_accept_call();

		// Fork a process here.
		pid = fork();
		if(pid == 0)
		{
			char intBuf[BUFSIZE];
			close(sock);
			srand((unsigned)time(&t));
			clientName = receiveMessageFromClient(accept_client);
			if(validUsername(clientName))
			{
				int randNum = rand();
				sprintf(intBuf, "%d", randNum);
				sendMessageToClient(intBuf, accept_client);
			}
			else
			{
				sendMessageToClient("usernotfound", accept_client);
				close(accept_client);
			}
			char* rcvdPwd = receiveMessageFromClient(accept_client);
			// printf("Received password is %s\n", rcvdPwd);
			// Check whether the password is correct.
			char* corrPwd = validPassword(clientName);
			char* keyStr = concat(corrPwd, intBuf);
			char* cryptKey = crypt(keyStr, "salt");
			// printf("Correct key should be: %s\n", cryptKey);
			if(strcmp(cryptKey, rcvdPwd) == 0)
			{
				sendMessageToClient("keycorrect", accept_client);
				printf("%s is accessing information!\n", clientName);
			}
			else
			{
				sendMessageToClient("keyerror", accept_client);
				close(accept_client);
			}
			// Receive command from client.
			char* cmd = receiveMessageFromClient(accept_client);
			char* dupCmd = strdup(cmd);
			// printf("command received is %s\n", cmd);
			int cmdCnt = countInputs(dupCmd);
			// printf("cmdcnt is %d\n", cmdCnt);
			char* cmdArr[cmdCnt + 1];
			storeInputs(cmdArr, cmd, cmdCnt + 1);
			// printf("After function\n");
			cmdArr[cmdCnt] = (char*)NULL;
			// printf("After print\n");
			// Use dup2 to redirect output.
			dup2(accept_client, STDOUT_FILENO);
			dup2(accept_client, STDERR_FILENO);
			close(accept_client);
			int eRtn = execvp(*cmdArr, cmdArr);
			if(eRtn)
			{
				perror("execvp()");
				exit(-1);
			}
		}
		else
		{
			// close accept_client.
			close(accept_client);

			// call waitpid to free resources.
			waitpid_call = waitpid(-1, &status, WNOHANG);
			// Free all finished processes.
			ERROR_waitpid_call();
			continue;
		}
	}
}

/* counts commands from command line */
int countInputs(char* cmd)
{
	int cmdCnt = 0;
	char* token;
	token = strtok(cmd, " ");
	while(token != NULL)
	{
		cmdCnt ++;
		token = strtok(NULL, " ");
	}
	return cmdCnt;
}

/* stores commands from command line */
void storeInputs(char** cmdArr, char* cmd, int arrSize)
{
	int i = 0;
	char* token;
	token = strtok(cmd, " ");
	cmdArr[i] = strdup(token);
	while(token != NULL)
	{
		i ++;
		token = strtok(NULL, " ");
		if(token == NULL)
		{
			break;
		}
		// cmdArr[i] = strdup(token);
		cmdArr[i] = token;
	}
	return;
}

/* send message to client */
int sendMessageToClient(char* msg, int sock)
{
	char termiStr[2] = {4, '\0'};
	if(write(sock, msg, strlen(msg)) == -1)
	{
		perror("write()");
		exit(-1);
	}
	if(write(sock, termiStr, strlen(termiStr)) == -1)
	{
		perror("write()");
		exit(-1);
	}
	return 0;
}
/* receive message from client */
char* receiveMessageFromClient(int sock)
{
	char msgBuf[BUFSIZE];
	char* msg = "";
	while(1)
	{
		bytes = read(sock, msgBuf, BUFSIZE);
		if(bytes == 0)
		{
			printf("Client disconnected!\n");
			exit(-1);
		}
		else if(bytes < 0)
		{
			printf("%s entered password incorrectly.\n", clientName);
			exit(-1);
		}
		else
		{
			int termLoc = containToken(msgBuf, bytes);
			if(termLoc >= 0)
			{
				msgBuf[termLoc] = '\0';
				msg = concat(msg, msgBuf);
				return msg;
			}
			else
			{
				msgBuf[bytes] = '\0';
				msg = concat(msg, msgBuf);
			}
		}
	}

}

/* checks username */
char* validUsername(char* name)
{
	int i;
	for(i = 0; i < USERCNT; i ++)
	{
		if(strcmp(name, valid_username[i]) == 0)
		{
			return valid_username[i];
		}
	}
	return "";
}
/* checks password */
char* validPassword(char* clientName)
{
	int i;
	for(i = 0; i < USERCNT; i ++)
	{
		if(strcmp(clientName, valid_username[i]) == 0)
		{
			return valid_password[i];
		}
	}
	return "";
}

/* set server to connect to */
void setServer()
{	
	bzero((char*) &server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);
	
	ERROR_socket_call();
	ERROR_bind_call();
	ERROR_listen_call();
}



/* ERROR HANDLERS */

/* bind */
void ERROR_bind_call()
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


/* SIMILAR FUNCTIONS */

/* usage message */
void usage()
{
	printf("******** Distributed Shell Server ********\n");
	printf("Jonathan Metzger ~ April 5th 2018 ~ CS4513\n");
	printf("usage: ./server [flags]\n");
	printf("  -h usage message\n");
	printf("  -p (optional) port: default is 4513\n");
	printf("******************************************\n");
}


/* checks flag of input */
void flagCheck(int argc, char **argv)
{
    while ((option = getopt (argc, argv, "hp:")) != -1)
    {
        switch (option)
        {
       		/* -p port */
            case 'p':
            port = atoi(optarg);
            break;

            /* -h help message */
            case 'h':
			usage();
			exit(1);
            break;

            default:
            exit(-1);

        }
    }
}
