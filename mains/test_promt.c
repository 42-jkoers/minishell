#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "env.h"

void	print(const t_list *cmd)
{
	size_t	i;
	char	**blocks;

	blocks = cmd->data;
	i = 0;
	printf("%lu {", cmd->count);
	while (i < cmd->count)
	{
		printf("\"%s\"", blocks[i]);
		if (i + 1 != cmd->count)
			printf(", ");
		i++;
	}
	printf("}\n");
}

int	main(int argc, char** argv, const char** envp)
{
	t_list	cmd;

	(void)argc;
	(void)argv;
	env_ptr_copy(envp);
	while (true)
	{
		cmd = command_read();
		print(&cmd);
		// list_un_init(&cmd, free);
	}
}
