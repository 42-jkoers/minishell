#include "executable.h"
#include "working_directory.h"
#include "env.h"
#include "libft.h"

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void log_status(int status)
{
	if (WIFEXITED(status))
		printf("	exit status: %i\n", WEXITSTATUS(status));
	else
		printf("	unknown exit status...\n");
}

int	main(int argc, char** argv, const char** envp)
{
	env_ptr_copy(envp);
	(void)argc;
	(void)argv;

	t_executable	executable;
	pid_t			pid;
	int				status;

	printf("current work directory: %s\n", working_directory_get());

	// cd mains
	printf("running 'cd mains'\n");
	executable_init(&executable, "cd");
	executable_add_arg(&executable, "mains");
	pid = executable_run(&executable);
	waitpid(pid, &status, 0);
	printf("	current work directory: %s\n", working_directory_get());
	log_status(status);

	printf("running 'cd mains toomanyargs'\n");
	executable_add_arg(&executable, "toomanyargs");
	pid = executable_run(&executable);
	waitpid(pid, &status, 0);
	printf("	current work directory: %s\n", working_directory_get());
	log_status(status);

	executable_un_init(&executable);
	executable_init(&executable, "cd");

	printf("running 'cd ../src'\n");
	executable_add_arg(&executable, "../src");
	pid = executable_run(&executable);
	waitpid(pid, &status, 0);
	printf("	current work directory: %s\n", working_directory_get());
	log_status(status);

	executable_un_init(&executable);
	executable_init(&executable, "cd");

	printf("running 'cd this_directory_does_not_exist'\n");
	executable_add_arg(&executable, "this_directory_does_not_exist");
	pid = executable_run(&executable);
	waitpid(pid, &status, 0);
	printf("	current work directory: %s\n", working_directory_get());
	log_status(status);

	executable_un_init(&executable);
}
