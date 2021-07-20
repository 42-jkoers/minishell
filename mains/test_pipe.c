#include "executable.h"
#include "env.h"

#include <sys/wait.h>
#include <unistd.h>
#include "utils.h"

// ls | grep test | wc -l

int	main(int argc, char** argv, const char** envp)
{
	env_copy_ptr(envp);
	(void)argc;
	(void)argv;

	//working_directory_set("/home");

	t_executable	executables[3];
	pid_t			pids[3];
	int				status;

	executable_init(&executables[0], "ls");
	executable_init(&executables[1], "grep");
	executable_init(&executables[2], "wc");

	executable_add_arg(&executables[1], "test");
	executable_add_arg(&executables[2], "-l");

	executable_add_pipe(&executables[0], &executables[1]);
	executable_add_pipe(&executables[1], &executables[2]);

	stupid_write(STDOUT_FILENO, "Should output the number of files with the name \"test\" in the current directory\n", 80);
	pids[0] = executable_run(&executables[0]);
	pids[1] = executable_run(&executables[1]);
	pids[2] = executable_run(&executables[2]);

	waitpid(pids[0], &status, 0);
	waitpid(pids[1], &status, 0);
	waitpid(pids[2], &status, 0);

	executable_un_init(&executables[0]);
	executable_un_init(&executables[1]);
	executable_un_init(&executables[2]);
}
