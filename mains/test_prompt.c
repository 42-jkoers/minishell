#include <stdio.h>
#include "env.h"
#include "command_read.h"

void	print(const t_list *cmd)
{
	size_t	i;
	t_block	*block;

	i = 0;
	printf("%lu {", cmd->count);
	while (i < cmd->count)
	{
		block = list_index_unchecked(cmd, i);
		printf("`%s`", block->text);
		if (i + 1 != cmd->count)
			printf(", ");
		i++;
	}
	printf("}\n");
}

int	main(int argc, char **argv, const char **envp)
{
	t_list	cmd;

	(void)argc;
	(void)argv;
	env_copy_ptr(envp);
	while (true)
	{
		cmd = command_read();
		print(&cmd);
		command_read_destroy(&cmd);
	}
}
