#include "executable.h"
#include "env.h"
#include "utils.h"

#include <sys/wait.h>
#include <unistd.h>

// ls | grep test | wc -l

int	main(int argc, char** argv, const char** envp)
{
	env_copy_ptr(envp);
	(void)argc;
	(void)argv;

	//working_directory_set("/home");

	t_executable	executable;
	pid_t			pid;
	int				status;

	executable_init(&executable, "cat");

	stupid_write(STDOUT_FILENO, "Should echo back what you write after you submit the delimiter: 'eof'\n", 70);
	executable_add_here_doc(&executable, "eof");

	pid = executable_run(&executable);

	waitpid(pid, &status, 0);

	executable_un_init(&executable);
}
