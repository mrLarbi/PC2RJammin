#include "input.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "tools.h"
#include "output.h"
#include "session.h"
#include "sound.h"




void handle_welcome(client_core_t core, char* user){
	printf("Welcome :%s\n", user);
	strcpy(core->user, user);
	printf("Trying to connect...\n");
	wait_for_specific_answer(core,core->main_socket,  ANS_AUDIO_PORT);
}

void handle_audio_port(client_core_t core, char* port){
	core->second_port = atoi(port);
}

void handle_audio_ok(client_core_t core){
	
}

void handle_connected(client_core_t core, char* user){
	printf("You are connected\n");
}

void handle_exited(client_core_t core, char* user){
	printf("%s exits.\n", user);
	if(!strcmp(core->user, user))
	{
		core->cont = 0;	
	}
	else{
		core->session->nbMus--;
	}
}

void handle_empty_session(client_core_t core){
	printf("---EMPTY SESSION---\n");
	printf("Please enter style : ");
	char style[STRING_SZ];
	read_string(style);
	printf("Please enter temp : ");
	char tempo[STRING_SZ];
	read_string(tempo);
	core->session = create_session(0, style, atoi(tempo));
	send_set_options(core->main_socket, style, tempo); // Changed
}

void handle_current_session(client_core_t core, char* style, char* temp, char* nbMus){
	printf("Current session : \nStyle : %s\nTempo : %s\nNumber of musicians : %s\n", style, temp, nbMus);
	core->session = create_session(atoi(nbMus), style, atoi(temp));
}

void handle_ack_opts(client_core_t core){
	printf("Options received\n");
}

void handle_full_session(client_core_t core){
	printf("FULL_SESSION\n");
	send_exit(core->main_socket, core->user);
	wait_for_specific_answer(core, core->main_socket, ANS_EXITED);
}

void handle_audio_ko(client_core_t core){
}

void handle_audio_mix(client_core_t core, char* buff){
	copy_all(core->playback->buffer, buff,1+core->playback->size);
	play_sound(core->playback);
	send_audio_ack(core->main_socket);
}
 
void handle_chat_listen(client_core_t core, char* sender, char* text){
	printf("\n%s : %s\n",sender,text);	// Pour le chat 
	char msg[STRING_SZ];
	sprintf(msg, "%s : ", core->user);
	write(1, msg, strlen(msg));
}

void handle_audio_sync(client_core_t core, char* tick){
	core->tick = atof(tick);	// Current server's tick.
}
