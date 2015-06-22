#include "command_handler.h"
#include "tools.h"
#include <stdio.h>


void test_parsing_req_to_string()
{
	printf("----------Req Parsing To String Test----------\n");
	int i ;
	for (i = 0; i < REQ_NB; i += 1)
	{
		printf("num : %d , string : %s\n", i, parse_request_to_string(i));	
	}
	printf("num : %d , string : %s\n", i, parse_request_to_string(i));
}

int main(int argc, char** argv)
{
	test_parsing_req_to_string();
	char string[STRING_SZ];
	while(1)
	{
		read_string(string);
		printf("Commande : %s\n", string);
		command_t commande = parse_command(string);
		printf("Requete : %s\n", parse_request_to_string(commande.req));
		int i;
		for (i = 0; i < commande.nb_args; i += 1)
		{
			printf("Argument n°%d : %s\n", i+1, commande.arguments[i]);
		}
	}
	return 0; //Should never come here 
}
