#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include "signal_handler.h"

int main()
{
	setup_signals();

	while (true)
	{
		char* cmd = readline("minishell$ ");
		if (cmd == NULL)
			break;
		printf("Got string: '%s'\n", cmd);
		add_history(cmd);
		free(cmd);
	}
	printf("\n");
}