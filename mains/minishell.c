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

static void	executable_from_command(const t_list *cmd, t_executable *
	o_executable)
{
	size_t	i;

	executable_init(o_executable, *(char **)list_index(cmd, 0));
	i = 1;
	while (i < cmd->count)
	{
		executable_add_arg(o_executable, *(char **)list_index(cmd, i));
		i++;
	}
}

//list_un_init(&cmd, free);
//list_un_init(&cmd, free);

int	main(int argc, char **argv, const char **envp)
{
	t_executable	executable;
	pid_t			pid;
	t_list			cmd;
	int				status;

	env_copy_ptr(envp);
	(void)argc;
	(void)argv;
	while (true)
	{
		cmd = command_read();
		executable_from_command(&cmd, &executable);
		list_un_init_ptr(&cmd);
		pid = executable_run(&executable);
		waitpid(pid, &status, 0);
		executable_un_init(&executable);
	}
}
