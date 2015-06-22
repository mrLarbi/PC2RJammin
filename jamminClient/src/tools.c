#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"

void error_argument_message()
{
	fprintf(stderr, "Usage : jamminClient [-port <port>] [-user <name>] [-address <server_address>]\n");
}

void copy_all(char* dest, char* src, int size)
{
	int i;
	for (i = 0; i < size; i += 1)
	{

		dest[i] = src[i];

	}
}


client_core_t parse_arguments(int argc, char** argv)
{
	client_core_t core = NULL;
	
	char user[STRING_SZ] = "pc2r";
	char port[STRING_SZ] = "2015";
	char address[STRING_SZ] = "localhost";
	
	int i = 0;
	for (i = 1; i < argc; i += 2)
	{
		if (!strcmp(argv[i], "-user"))
		{
			strcpy(user, argv[i+1]);
		}
		else if (!strcmp(argv[i], "-port"))
		{
			strcpy(port, argv[i+1]);
		}
		else if (!strcmp(argv[i], "-address"))
		{
			strcpy(address, argv[i+1]);
		}
	}
	core = create_client_core(user, atoi(port), address);
	return core;
}

void flush_buffer()
{
    int c = 0;
    while ((c = getchar()) && c != '\n' && c != EOF);
}

int read_string(char* string)
{
	if(fgets(string, STRING_SZ, stdin))
	{
		char* crlf;
		if((crlf = strchr(string, '\n')))
		{
			*crlf = '\0';
		}
		return 0;
	}
	return -1;
}
