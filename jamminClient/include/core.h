#ifndef __CORE_H__
#define __CORE_H__

#include "command_handler.h"
#include "const.h"
#include "session.h"
#include "sound.h"


typedef struct client_core_t 
{
	char address[STRING_SZ];
	int main_socket;
	int second_socket;
	char user[STRING_SZ];
	int main_port;
	int second_port;
	int cont;
	float tick;   // Le tick envoyer par le serveur
	sound_t capture;
	sound_t playback;
	session_t session;
}*client_core_t;

client_core_t create_client_core(char* user, int port, char* address);
void destroy_client_core(client_core_t core);
void run_client(client_core_t core);
void handle_answer(client_core_t core, command_t *command);
command_t wait_for_answer(int sock);
int true_connection(client_core_t core);
void wait_for_specific_answer(client_core_t core,int sock, request_t ans);

#endif /* __CORE_H__ */

