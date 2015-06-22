#include "output.h"
#include "connection.h"


void send_connect(int sock, char* user)
{
	send_msg(sock , "CONNECT/");
	send_msg(sock , user);
	send_msg(sock , "/\n");	
}

void send_set_options(int sock, char* style, char* tempo)
{
	send_msg(sock , "SET_OPTIONS/");
	send_msg(sock , style);
	send_msg(sock, "/");
	send_msg(sock , tempo);
	send_msg(sock, "/\n");
}

void send_audio_chunk(int sock, char* tick, char* buff)
{
	send_msg(sock , "AUDIO_CHUNK/");
	send_msg(sock , tick);
	send_msg(sock, "/");
	//send_msg(sock , buff);
	write(sock, buff, 128);
	send_msg(sock, "/");
	send_msg(sock, "\n");
}

void send_audio_ack(int sock)
{
	send_msg(sock , "AUDIO_ACK");
	send_msg(sock, "/");
	send_msg(sock, "\n");
}

void send_exit(int sock, char* user)
{
	send_msg(sock, "EXIT/");
	send_msg(sock , user);
	send_msg(sock, "/");
	send_msg(sock, "\n");
}

void send_text(int sock, char* text)
{
	send_msg(sock, "TALK/");
	send_msg(sock , text);
	send_msg(sock, "/");
	send_msg(sock, "\n");
}

void send_audio_size(int sock, char* buff_size)
{
	send_msg(sock, "AUDIO_SIZE/");
	send_msg(sock , buff_size);
	send_msg(sock, "/");
	send_msg(sock, "\n");
}
