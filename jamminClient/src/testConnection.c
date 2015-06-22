#include "connection.h"
#include <stdio.h>

int main (int argc, char **argv)
{
	int sock = connect_to_server("localhost", 2015);
	send_msg(sock, "Luis-Suarez");
	char string[100];
	read_msg(sock, string, 100);
	printf("%s\n", string);
	disconnect_from_server(sock);
	return 0;
}
