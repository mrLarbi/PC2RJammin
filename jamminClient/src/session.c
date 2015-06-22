#include "session.h"
#include <stdlib.h>
#include <string.h>


session_t create_session(int nbMus, char* style, int tempo)
{
	session_t session = malloc(sizeof(struct session_t));
	session->nbMus = nbMus;
	strcpy(session->style, style);
	session->tempo = tempo;
	return session;
}

void destroy_session(session_t session)
{
	free(session);
}

