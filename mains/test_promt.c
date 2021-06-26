#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

void	print(void *s)
{
	printf("> %s\n", (char *)s);
}

int	main(void)
{
	t_list	cmd;

	while (true)
	{
		cmd	= command_read();
		list_foreach(&cmd, print);
	}
}
