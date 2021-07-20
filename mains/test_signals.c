#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include "signal_handler.h"
#include "readline_ext.h"

int main()
{
	setup_signals();

	while (true)
	{
		char* cmd;
		t_readline_ret_type ret = readline_ext("minishell$ ", &cmd);
		printf("Got code: %i and string: '%s'\n", ret, cmd);
		if (cmd == NULL)
			break;
		add_history(cmd);
		free(cmd);
	}
	printf("\n");
}