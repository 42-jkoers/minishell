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

static void executable_from_command(const t_list* cmd, t_executable* o_executable)
{
	executable_init(o_executable, *(char**)list_index(cmd, 0));
	for (size_t i = 1; i < cmd->count; i++)
		executable_add_arg(o_executable,  *(char**)list_index(cmd, i));
}

int	main(int argc, char** argv, const char** envp)
{
	env_copy_ptr(envp);
	(void)argc;
	(void)argv;


	t_list	cmd;
	t_executable executable;

	while (true)
	{
		cmd = command_read();
		if (cmd.count < 1)
		{
			//list_un_init(&cmd, free);
			continue;
		}
		executable_from_command(&cmd, &executable);

		pid_t pid = executable_run(&executable);
		int status;
		waitpid(pid, &status, 0);

		executable_un_init(&executable);
		//list_un_init(&cmd, free);
	}
}
