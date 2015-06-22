#ifndef __COMMANDE_HANDLER_H__
#define __COMMANDE_HANDLER_H__

#include <stdio.h>
#include <string.h>

#include "const.h"


typedef enum { ANS_WELCOME = 0, 
					ANS_AUDIO_PORT , 
					ANS_AUDIO_OK, 
					ANS_AUDIO_KO, 
					ANS_CONNECTED,
					ANS_EXITED, 
					ANS_EMPTY_SESSION, 
					ANS_CURRENT_SESSION, 
					ANS_ACK_OPTS, 
					ANS_FULL_SESSION, 
					ANS_AUDIO_MIX,
					ANS_LISTEN,
					ANS_AUDIO_SYNC,
					
					REQ_TALK, 
					REQ_CONNECT, 
					REQ_SET_OPTIONS, 
					REQ_AUDIO_CHUNK, 
					REQ_AUDIO_ACK,
					REQ_EXIT, 
					
					REQ_INVALID,
					REQ_NB
					
					} request_t;

typedef struct command_t
{
	request_t req;
	char arguments[MAX_ARGS][STRING_SZ];
	int nb_args;
}command_t;

command_t parse_command(char* command);
const char* parse_request_to_string(request_t req);
int check_command(command_t *command);

#endif /* __COMMANDE_HANDLER_H__ */

