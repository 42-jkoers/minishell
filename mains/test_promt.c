#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int	main(int argc, char **argv)
{
	char	*read;

	(void)argc;
	(void)argv;
	while (true)
	{
		command_read();
	}
}
