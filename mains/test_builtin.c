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
	{
		int exit_code = WEXITSTATUS(status);
		printf("    exit status: \e[%im%i\e[0m\n", exit_code ? 31 : 32, exit_code);
	}
	else
		printf("    \e[31munknown exit status...\e[0m\n");
}

static void log_arg(const char** arg)
{
	printf(" %s", *arg);
}

static void run(t_executable* executable)
{
	printf("\e[32mrunning executable:\e[33m");
	list_foreach(&executable->args, (t_foreach_value)log_arg);
	printf("\e[0m\n");


	pid_t pid = executable_run(executable);
	if (pid == -1)
	{
		printf("executable_run returned -1!\n");
		exit(1);
	}
	int status;
	waitpid(pid, &status, 0);
	log_status(status);
}

static void reset_executable(t_executable* executable, const char* new_command)
{
	executable_un_init(executable);
	executable_init(executable, new_command);
}

int	main(int argc, char** argv, const char** envp)
{
	env_copy_ptr(envp);
	(void)argc;
	(void)argv;

	t_executable	pwd_executable;
	t_executable	executable;


	executable_init(&pwd_executable, "pwd");


	// env
	executable_init(&executable, "env");
	run(&executable);

	// ls
	reset_executable(&executable, "ls");
	run(&executable);

	// unset PATH
	reset_executable(&executable, "unset");
	executable_add_arg(&executable, "PATH");
	run(&executable);

	// unset PATH "Bad name"
	executable_add_arg(&executable, "Bad name");
	run(&executable);

	// ls
	reset_executable(&executable, "ls");
	run(&executable);

	// env
	reset_executable(&executable, "env");
	run(&executable);

	run(&pwd_executable);

	// cd mains
	reset_executable(&executable, "cd");
	executable_add_arg(&executable, "mains");
	run(&executable);
	run(&pwd_executable);

	executable_add_arg(&executable, "toomanyargs");
	run(&executable);
	run(&pwd_executable);

	reset_executable(&executable, "cd");
	executable_add_arg(&executable, "../src");
	run(&executable);
	run(&pwd_executable);

	reset_executable(&executable, "cd");
	executable_add_arg(&executable, "this_directory_does_not_exist");
	run(&executable);
	run(&pwd_executable);

	reset_executable(&executable, "cd");
	run(&executable);
	run(&pwd_executable);

	reset_executable(&executable, "cd");
	executable_add_arg(&executable, "/dev");
	run(&executable);
	run(&pwd_executable);

	executable_un_init(&executable);
	executable_un_init(&pwd_executable);
}
