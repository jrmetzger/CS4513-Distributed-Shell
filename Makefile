all: client server

client: client.c
	gcc -g3 -o client client.c -lcrypt

server: server.c
	gcc -g3 -o server server.c -lcrypt

clean:
	rm client server


