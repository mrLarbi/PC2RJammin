#include "connection.h"
#include "output.h"
#include <stdio.h>


void testOutput(int sock)
{
	send_connect(sock, "Luis-Suarez");
	send_set_options(sock, "You'll-never-walk-alone", "Rapide");
	send_audio_chunk(sock, "5", "CL");
	send_audio_ack(sock);
	send_exit(sock, "Luis-Suarez");
}

int main (int argc, char **argv)
{
	int sock = connect_to_server("127.0.0.1", 2015);
	testOutput(sock);
	return 0;
}
