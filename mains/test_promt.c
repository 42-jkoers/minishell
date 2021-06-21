#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	while (true)
	{
		read_command();
	}
}
