#include "command_handler.h"
#include "core.h"
#include "tools.h"

request_t parse_command_name(char* command)
{
	if(!strcmp(command, "ACK_OPTS"))
	{
		return ANS_ACK_OPTS;
	}
	if(!strcmp(command, "AUDIO_KO"))
	{
		return ANS_AUDIO_KO;
	}
	if(!strcmp(command, "AUDIO_MIX"))
	{
		return ANS_AUDIO_MIX;
	}
	if(!strcmp(command, "AUDIO_OK"))
	{
		return ANS_AUDIO_OK;
	}
	if(!strcmp(command, "AUDIO_PORT"))
	{
		return ANS_AUDIO_PORT;
	}
	if(!strcmp(command, "CONNECTED"))
	{
		return ANS_CONNECTED;
	}
	if(!strcmp(command, "CURRENT_SESSION"))
	{
		return ANS_CURRENT_SESSION;
	}
	if(!strcmp(command, "EMPTY_SESSION"))
	{
		return ANS_EMPTY_SESSION;
	}
	if(!strcmp(command, "EXITED"))
	{
		return ANS_EXITED;
	}
	if(!strcmp(command, "FULL_SESSION"))
	{
		return ANS_FULL_SESSION;
	}
	if(!strcmp(command, "WELCOME"))
	{
		return ANS_WELCOME;
	}
	
	if(!strcmp(command, "LISTEN"))
	{
		return ANS_LISTEN;
	}
	if(!strcmp(command, "TALK"))
	{
		return REQ_TALK;
	}
	
	if(!strcmp(command, "AUDIO_SYNC"))
	{
		return ANS_AUDIO_SYNC;
	}
	
	if(!strcmp(command, "AUDIO_ACK"))
	{
		return REQ_AUDIO_ACK;
	}
	if(!strcmp(command, "AUDIO_CHUNK"))
	{
		return REQ_AUDIO_CHUNK;
	}
	if(!strcmp(command, "SET_OPTIONS"))
	{
		return REQ_SET_OPTIONS;
	}
	return REQ_INVALID;
}

void shift_characters(char* string, int start, int end)
{
	int i = start;
	for( ; i < end ; i++)
	{
		string[i] = string[i+1];
	}
}

void escape_special_char(char* string)
{
	int i = 0;
	int sizeCommand = strlen(string);
	for (i = 0; i < sizeCommand; i += 1)
	{
		if(string[i] == '\\')
		{
			if(i < sizeCommand - 1)
			{
				i++;
				if(string[i] == '\\' || string[i] == '/')
				{
					shift_characters(string , i-1 , sizeCommand + 1);
				}
			}
		}
	}
}

void get_before_sep(char* string, char** left)
{
	int i;
	int sizeCommand = strlen(string);
	for (i = 0; i < sizeCommand; i += 1)
	{
		if(string[i] == '/')
		{
			if(i < sizeCommand -1)
				*(left) = string + (i + 1);
			string[i] = '\0';
			return;
		}
	}
}

void parse_command_arguments(command_t* comm, char* arguments)
{
	char* left = NULL;
	while(arguments)
	{
		if(comm->nb_args == MAX_ARGS)
		{
			fprintf(stderr, "Too many arguments\n");
			return;	
		}
		get_before_sep(arguments, &left);
		strcpy(comm->arguments[comm->nb_args],arguments);
		arguments = left;
		left = NULL;
		comm->nb_args++;
	}
}

command_t parse_command(char* command)
{
	command_t comm;
	comm.nb_args = 0;
	escape_special_char(command);
	char* arguments = NULL;
	get_before_sep(command, &arguments);
	if((comm.req = parse_command_name(command)) == REQ_INVALID)
	{
		return comm;
	}
	parse_command_arguments(&comm, arguments);
	if(!check_command(&comm))
	{
		comm.req = REQ_INVALID;
		comm.nb_args = 0;
	}
	return comm;
}

const char* parse_request_to_string(request_t req)
{
	const char* request_string[REQ_NB] = {"WELCOME",
	"AUDIO_PORT" , 
	"AUDIO_OK", 
	"AUDIO_KO", 
	"CONNECTED",
	"EXITED", 
	"EMPYT_SESSION", 
	"CURRENT_SESSION", 
	"ACK_OPTS", 
	"FULL_SESSION", 
	"AUDIO_MIX",
	"LISTEN",
	"AUDIO_SYNC",
	
	
	"TALK",
	"CONNECT", 
	"SET_OPTIONS", 
	"AUDIO_CHUNK", 
	"AUDIO_ACK",
	"EXIT", 
	"INVALID"

	};
	if(req >= REQ_NB || req < 0)
	{
		fprintf(stderr, "parsing bad request number to string, get out of here !\n");
		return NULL;
	}
	return request_string[req];
}

int check_ack_opts(command_t *command)
{
	if(command->nb_args != 0)
	{
		fprintf(stderr, "Wrong ACK_OPTS, should be : ACK_OPTS\n");
		return 0;
	}
	return 1;
}

int check_audio_ack(command_t *command)
{
	if(command->nb_args != 0)
	{
		fprintf(stderr, "Wrong AUDIO_ACK, should be : AUDIO_ACK\n");
		return 0;
	}
	return 1;
}

int check_audio_chunk(command_t *command)
{
	if(command->nb_args != 2)
	{
		fprintf(stderr, "Wrong AUDIO_CHUNK, should be : AUDIO_CHUNK/tick/buff\n");
		return 0;
	}
	return 1;
}

int check_audio_ko(command_t *command)
{
	if(command->nb_args != 0)
	{
		fprintf(stderr, "Wrong AUDIO_KO, should be : AUDIO_KO\n");
		return 0;
	}
	return 1;
}

int check_audio_mix(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong AUDIO_MIX, should be : AUDIO_MIX/buff\n");
		return 0;
	}
	return 1;
}

int check_audio_ok(command_t *command)
{
	if(command->nb_args != 0)
	{
		fprintf(stderr, "Wrong AUDIO_OK, should be : AUDIO_OK\n");
		return 0;
	}
	return 1;
}

int check_audio_port(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong AUDIO_PORT, should be : AUDIO_PORT/port\n");
		return 0;
	}
	return 1;
}

int check_connect(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong CONNECT, should be : CONNECT/user\n");
		return 0;
	}
	return 1;
}

int check_connected(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong CONNECTED, should be : CONNECTED/user\n");
		return 0;
	}
	return 1;
}

int check_current_session(command_t *command)
{
	if(command->nb_args != 3)
	{
		fprintf(stderr, "Wrong CURRENT_SESSION, should be : CURRENT_SESSION/style/tempo/nbMus\n");
		return 0;
	}
	return 1;
}

int check_empty_session(command_t *command)
{
	if(command->nb_args != 0)
	{
		fprintf(stderr, "Wrong EMPTY_SESSION, should be : EMPTY_SESSION\n");
		return 0;
	}
	return 1;
}

int check_exit(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong EXIT, should be : EXIT/user\n");
		return 0;
	}
	return 1;
}

int check_exited(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong EXITED, should be : EXITED/user\n");
		return 0;
	}
	return 1;
}

int check_full_session(command_t *command)
{
	if(command->nb_args != 0)
	{
		fprintf(stderr, "Wrong FULL_SESSION, should be : FULL_SESSION\n");
		return 0;
	}
	return 1;
}

int check_set_options(command_t *command)
{
	if(command->nb_args != 2)
	{
		fprintf(stderr, "Wrong SET_OPTIONS, should be : SET_OPTIONS/style/temp\n");
		return 0;
	}
	return 1;
}

int check_welcome(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong WELCOME, should be : WELCOME/user\n");
		return 0;
	}
	return 1;
}

int check_listen(command_t *command)
{
	if(command->nb_args !=2)
	{
		fprintf(stderr, "Wrong LISTEN, should be : LISTEN/sender/texte\n");
		return 0;
	}
	return 1;
}

int check_talk(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong TALK, should be : TALK/texte\n");
		return 0;
	}
	return 1;
}

int check_audio_sync(command_t *command)
{
	if(command->nb_args != 1)
	{
		fprintf(stderr, "Wrong AUDIO_SYNC, should be : AUDIO_SYNC/tick\n");
		return 0;
	}
	return 1;
}


int check_command(command_t *command)
{
	switch(command->req)
	{
		case ANS_ACK_OPTS:
			return check_ack_opts(command);
		case ANS_AUDIO_KO:
			return check_audio_ko(command);
		case ANS_AUDIO_MIX:
			return check_audio_mix(command);
		case ANS_AUDIO_OK:
			return check_audio_ok(command);
		case ANS_AUDIO_PORT:
			return check_audio_port(command);
		case ANS_CONNECTED:
			return check_connected(command);
		case ANS_CURRENT_SESSION:
			return check_current_session(command);
		case ANS_EMPTY_SESSION:
			return check_empty_session(command);
		case ANS_EXITED:
			return check_exited(command);
		case ANS_FULL_SESSION:
			return check_full_session(command);
		case ANS_WELCOME:
			return check_welcome(command);
		case ANS_LISTEN:
			return check_listen(command);
		case REQ_TALK:
			return check_talk(command);	
		case ANS_AUDIO_SYNC:
			return check_audio_sync(command);		
		case REQ_AUDIO_ACK:
			return check_audio_ack(command);
		case REQ_AUDIO_CHUNK:
			return check_audio_chunk(command);
		case REQ_CONNECT:
			return check_connect(command);
		case REQ_SET_OPTIONS:
			return check_set_options(command);
		case REQ_EXIT:
			return check_exit(command);
		default:
			fprintf(stderr, "Invalid command\n");
			return 0;
	}
}

/**
 * 
 * Ligne 409 -414 ce que j'ai ajouter pour le chat et AUDIO_SYNC
 * 
 */
