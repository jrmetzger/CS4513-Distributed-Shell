
/*
 * client.c
 * 
 * Jonathan Metzger
 * Spring 2018
 *
 * Project for CS4513 Distributed Computing Systems
 *
 */

#include "common.h"
#include "common.c"

unsigned long int inAddr;

int bytes;
int port = PORT;
int terminatorLocation;
int w_flag;

struct hostent *host;

char* server_address="";
char* username="";
char* password;
char* salt = "salt";
char* command = "";
char* check_username;
char* password_key;
char* password_encrypt;
char* check_password;
char messageBuf[BUFSIZE];
char* message = "";
char* symbol = "@";

/* main */
int main(int argc, char** argv)
{
	/* flag input */
	flagCheck(argc, argv);

	/* talk to server */
	checkServer();
	checkCredentials();

	/* check the server */
	submitInput(sock);
}


/* get the host or address */
void getHost()
{

	/* free */
	bzero((void *)&serv, sizeof(serv));

	host = gethostbyname(server_address);
	if(host == NULL)
	{
		//printf("HELLO\n\n");
		perror("gethostbyname()");
		exit(-1);
	}

	/* copy */
	bcopy(host->h_addr, (char *)&serv.sin_addr, host->h_length);
}

/* check the server or host and connect via a socket*/
void checkServer()
{
	getHost();
	
	/* put in struct */
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);

	ERROR_socket_call();
	ERROR_connect_call();
}

/* send message */
int sendMessageToServer(char* message, int sock)
{
	char terminate_string[2] = {4, '\0'};
	if((write(sock, message, strlen(message)) == -1) || (write(sock, terminate_string, strlen(terminate_string)) == -1) )
	{
		perror("write()");
		exit(-1);
	}
	return 0;
}

/* get message from server */
char* receiveMessageFromServer(int sock)
{
	while(1)
	{
		bytes = read(sock, messageBuf, BUFSIZE);
		if(bytes == 0)
		{
			printf("\n** Client is now disconnected. **\n\n");
		}
		else if(bytes < 0)
		{
			perror("read()");
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

/* checks for valid username */
void checkUsername()
{
	int s = strcmp(check_username, "usernotfound");
	if(s == 0)
	{
		printf("\n** ERROR: username not found. **\n\n");
		exit(-1);
	}
	//printf("USERNAME IS:%d", s);
}

/* checks for valid password */
void checkPassword()
{
	if(strcmp(check_password, "keyerror") == 0)
	{
		fprintf(stderr, "\n** ERROR: Credentials don't match. Exiting... **\n\n");
		exit(-1);
	}
	else
	{
		printf("\n ** Password is correct. Accessing information. **\n\n");
		sendMessageToServer(command, sock);
	}
}

/* ask for username and checks if valid */
void checkCredentials()
{

	/* client send username to server (not password) */

	/* ask for username */
	sendMessageToServer(username, sock);
	check_username = receiveMessageFromServer(sock);
	checkUsername();

	printf("Username: %s\n", username);
	
	/* ask for password */
	if(!w_flag)
	{
		password = getpass("Password: ");
	}
	printf("Password: %s\n", password);

	password_key = concat(password, check_username);

	/* Client encrypts using user’s password plus number as key */
	password_encrypt = crypt(password_key, salt);

	/* Client sends hashed/encrypted value back to server */
	sendMessageToServer(password_encrypt, sock);
	
	free(check_username);
	free(password_key);

	check_password = receiveMessageFromServer(sock);
	checkPassword();
}

/* receive commands from command line */
int submitInput(int sock)
{
	while(1)
	{
		bytes = read(sock, messageBuf, BUFSIZE);
		if(bytes == 0)
		{
			return 0;
		}
		else if(bytes < 0)
		{
			perror("read()");
			exit(-1);
		}
		else
		{
			messageBuf[bytes] = '\0';
			printf("%s", messageBuf);
		}
	}
	close(sock);
}

/* SIMILAR FUNCTIONS */

/* usage message */
void usage()
{
	printf("******** Distributed Shell Client ********\n");
	printf("Jonathan Metzger ~ April 5th 2018 ~ CS4513\n");
	printf("usage: ./client [flags] {-c command}\n");
	printf("  -h usage message\n");
	printf("  -s server\n");
	printf("  -p (optional) port: default is 4513\n");
	printf("  -u username\n");
	printf("  -w password\n");
	printf("  -c 'command'\n");
	printf("******************************************\n");
}

/* checks flag of input */
void flagCheck(int argc, char **argv)
{
    while ((option = getopt (argc, argv, "hc:s:p:u:w:")) != -1)
    {
        switch (option)
        {

        	/* -c command */
            case 'c':
            command = optarg;
            break;

        	/* -s host */
            case 's':
            

        	/* read after "@" symbol */
        	
            if(strchr(optarg, *symbol) != NULL)
            {
            	server_address = strstr(optarg, symbol);
        	
        		if (server_address)
        		{ 
        			server_address++;
        		}
            }
            else
            {
	            server_address = optarg;
	        }
	        //printf("%s\n", server_address);
	        //exit(-1);
            break;

       		/* port */
            case 'p':
            port = atoi(optarg);
            break;

            /* help message */
            case 'h':
			usage();
			exit(1);
            break;

            /* set password */
            case 'w':
            password = optarg;
            w_flag++;
            break;

            /* set username */
            case 'u':
            username = optarg;
            break;

            default:
            exit(-1);

        }
    }

    if (strlen(command) == 0 || strlen(server_address) == 0 || strlen(username) == 0) {
		usage();
		exit(1);
	}
}
