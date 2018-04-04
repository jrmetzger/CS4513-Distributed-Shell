
/*
 * server.c
 * 
 * Jonathan Metzger
 * Spring 2018
 *
 * Project for CS4513 Distributed Computing Systems
 *
 */

#include "common.h"
#include "common.c"

/* set credentials */
char* valid_username[USERCNT] = {"user1", "user2", "user3", "user4", "user5"};
char* valid_password[USERCNT] = {"pass1", "pass2", "pass3", "pass4", "pass5"};

char* clientName = NULL;
char buffer[BUFSIZE];
char* token = NULL;
char* attemptPassword = NULL;
char* correctPassword = NULL;
char* keyPassword = NULL;
char* encryptKey = NULL;
char* command = NULL;
char* dupCommand = NULL;

time_t t;

struct sockaddr_in client_address;
struct sockaddr_in server_address;

int bytes = 0;
int pid = 0;
int port = PORT;
int randomNumber = 0;
int commandCount = 0;
int terminatorLocation = 0;
int client_length;

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

/* counts commands from command line */
int countInputs(char* command)
{
	token = strtok(command, " ");
	while(token != NULL)
	{
		commandCount ++;
		token = strtok(NULL, " ");
	}
	return commandCount;
}

/* stores commands from command line */
void storeInputs(char** commandArray, char* command, int arrSize)
{
	i = 0;
	token = strtok(command, " ");
	commandArray[i] = strdup(token);
	while(token != NULL)
	{
		i ++;
		token = strtok(NULL, " ");
		if(token == NULL)
		{
			break;
		}
		// commandArray[i] = strdup(token);
		commandArray[i] = token;
	}
	return;
}

/* set server to connect to */
void setServer()
{
	bzero((char*) &server_address, sizeof(server_address));

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);
	
	ERROR_socket_call();
	ERROR_bind_call(server_address);
	ERROR_listen_call();
}

/* open the server */
void openServer()
{
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
			close(sock);
			srand((unsigned)time(&t));
			getUsername();
			getPassword();
			getCommand();
		}
		else
		{
			close(accept_client);
			waitpid_call = waitpid(-1, &status, WNOHANG);
			ERROR_waitpid_call();
			continue;
		}
	}
}

/* send message to client */
int sendMessageToClient(char* message, int sock)
{
	char terminator[2] = {4, '\0'};
	if((write(sock, message, strlen(message)) == -1) || (write(sock, terminator, strlen(terminator)) == -1))
	{
		perror("write()");
		exit(-1);
	}
	return 0;
}
/* receive message from client */
char* receiveMessageFromClient(int sock)
{
	char messageBuf[BUFSIZE];
	char* message = "";
	while(1)
	{
		bytes = read(sock, messageBuf, BUFSIZE);
		if(bytes == 0)
		{
			printf("Client disconnected!\n");
			exit(-1);
		}
		else if(bytes < 0)
		{
			printf("'%s' entered password incorrectly.\n", clientName);
			exit(-1);
		}
		else
		{
			terminatorLocation = containToken(messageBuf, bytes);
			if(terminatorLocation >= 0)
			{
				messageBuf[terminatorLocation] = '\0';
				message = concat(message, messageBuf);
				return message;
			}
			else
			{
				messageBuf[bytes] = '\0';
				message = concat(message, messageBuf);
			}
		}
	}
}

/* get username from Client */
void getUsername()
{
	clientName = receiveMessageFromClient(accept_client);
	if(validUsername(clientName))
	{
		randomNumber = rand();
		sprintf(buffer, "%d", randomNumber);

		/* server responds by sending back unique random number */
		sendMessageToClient(buffer, accept_client);
	}
	else
	{
		sendMessageToClient("usernotfound", accept_client);
		exit(-1);
	}
}

/* checks username */
char* validUsername(char* name)
{
	for(i = 0; i < USERCNT; i ++)
	{
		if(strcmp(name, valid_username[i]) == 0)
		{
			return valid_username[i];
		}
	}
	return "";
}

/* get password from Client */
void getPassword()
{
	/* Server encrypts using the user’s same password plus number as key */
	attemptPassword = receiveMessageFromClient(accept_client);
	correctPassword = validPassword(clientName);
	keyPassword = concat(correctPassword, buffer);
	encryptKey = crypt(keyPassword, "salt");

	/* Server compares two hashed/encrypted values, if same then ok */
	if(strcmp(encryptKey, attemptPassword) == 0)
	{
		sendMessageToClient("keycorrect", accept_client);
	}
	else
	{
		sendMessageToClient("keyerror", accept_client);
		close(accept_client);
	}
}

/* checks password */
char* validPassword(char* clientName)
{
	for(i = 0; i < USERCNT; i ++)
	{
		if(strcmp(clientName, valid_username[i]) == 0)
		{
			return valid_password[i];
		}
	}
	return "";
}

/* get command from Client */
void getCommand()
{
	command = receiveMessageFromClient(accept_client);
	printf("'%s' is accessing information with the '%s' command!\n", clientName, command);
	dupCommand = strdup(command);
	commandCount = countInputs(dupCommand);
	char* commandArray[commandCount + 1];
	storeInputs(commandArray, command, commandCount + 1);
	commandArray[commandCount] = (char*)NULL;

	/* dup2 to redirect output */
	dup2(accept_client, STDOUT_FILENO);
	dup2(accept_client, STDERR_FILENO);

	close(accept_client);
	execvp_call = execvp(*commandArray, commandArray);
	ERROR_execvp_call();
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
