#ifndef __SESSION_H__
#define __SESSION_H__

#include "const.h"

typedef struct session_t
{
	int nbMus;
	char style[STRING_SZ];
	int tempo;
}*session_t;

session_t create_session(int nbMus, char* style, int tempo);
void destroy_session(session_t session);

#endif /* __SESSION_H__ */

