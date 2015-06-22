#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int connect_to_server(char* name, int port) {
	struct sockaddr_in dest;
	struct addrinfo* result;
	int sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

	if (getaddrinfo(name, 0, 0, &result) != 0) {
		perror("getaddrinfo");
		return -1;
	}
	memset((char*) &dest, 0, sizeof(dest));
	memcpy((void*) &((struct sockaddr_in*) result->ai_addr)->sin_addr,
			(void*) &dest.sin_addr, sizeof(dest));
	dest.sin_port = htons(port);
	dest.sin_family = AF_INET;
	if (connect(sock, (struct sockaddr*) &dest, sizeof(dest)) == -1) {
		perror("connect");
		return -1;
	}
	return sock;
}

int send_msg(int sock, char* msg) {
	return (write(sock, msg, strlen(msg)) == -1 ? -1 : 0);
}

int send_audio(int sock, char* buffer, int size) {
	return (write(sock, buffer, size) == -1 ? -1 : 0);
}

int read_msg(int sock, char* msg, int max_size) {
	int i;
	for (i = 0; i < max_size - 1; i += 1) {
		if (read(sock, &msg[i], 1) == -1) {
			return -1;
		}
		if (msg[i] == '\n') {
			msg[i] = '\0';
			return 0;
		}
	}
	msg[i] = '\0';
	return 0;
}

int read_all(int sock, char* msg, int max_size) {
	int i;
	for (i = 0; i < max_size; i += 1)
	{
		if (read(sock, &msg[i], 1) == -1)
		{
			return -1;
		}
	}
	return 0;

}

int disconnect_from_server(int sock) {
	if (shutdown(sock, 2) == -1) {
		return -1;
	}
	return close(sock);
}
