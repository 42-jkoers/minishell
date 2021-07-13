#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "env.h"
#include "minishell.h"
#include "executable.h"
#include "find_executable.h"
#include "command_read.h"
#include "malloc_wrappers.h"
#include "libft.h"

// TODO: exit code??
int	main(int argc, char **argv, const char **envp)
{
	t_list			cmd;
	t_command_type	type;

	env_copy_ptr(envp);
	(void)argc;
	(void)argv;
	while (true)
	{
		cmd = command_read();
		type = identify_command(&cmd);
		if (type == C_EXECTUTABLE)
			env_set("?", mc(ft_itoa(run_command_as_executable(&cmd))));
		command_read_destroy(&cmd);
	}
}
