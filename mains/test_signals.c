#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
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