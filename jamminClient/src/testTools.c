#include "tools.h"

void testArguments(int argc, char** argv)
{
	client_core_t core = parse_arguments(argc, argv);
	if(core)
	{
		printf("User : %s, Port : %d, Address : %s\n", core->user, core->main_port, core->address);	
	}
	else
	{
		printf("Arguments error, go and read the manuel :)\n");
	}
}

int main (int argc, char **argv)
{
	testArguments(argc, argv);
	return 0;
}
