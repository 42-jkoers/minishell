#include "command.h"
#include "working_directory.h"
#include "env.h"
#include "libft.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

// ls | grep test | wc -l

int	main(int argc, char** argv, char** envp)
{
	env_ptr_copy(envp);
	(void)argc;
	(void)argv;

	//working_directory_set("/home");

	t_command	commands[3];
	pid_t		pids[3];
	int			status;

	command_init(&commands[0], "ls");
	command_init(&commands[1], "grep");
	command_init(&commands[2], "wc");

	command_add_arg(&commands[1], "test");
	command_add_arg(&commands[2], "-l");

	command_add_pipe(&commands[0], &commands[1]);
	command_add_pipe(&commands[1], &commands[2]);

	pids[0] = command_run(&commands[0]);
	pids[1] = command_run(&commands[1]);
	pids[2] = command_run(&commands[2]);

	waitpid(pids[0], &status, 0);
	waitpid(pids[1], &status, 0);
	waitpid(pids[2], &status, 0);
}
