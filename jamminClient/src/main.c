#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "tools.h"
#include "connection.h"
#include "command_handler.h"


int main (int argc, char **argv)
{
	client_core_t core = parse_arguments(argc, argv);
	run_client(core);
	return 0;
}
