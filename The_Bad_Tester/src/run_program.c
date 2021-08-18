#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#include "JTester.h"
#include "pid_utils.h"
#include <errno.h>

extern char **environ;

int run_program_pid(float timeout, const char* path, const char* const* argv, const char* const* envp)
{
	pid_t pid = fork_with_timeout(timeout);
	if (pid == 0)
	{
		close_all_fds();
		if (envp == NULL)
			envp = (const char* const*)environ;
		// Add the path as argv[0]
		//	Count argc
		int argc = 0;
		if (argv)
			while (argv[argc])
				argc++;
		const char** new_argv = malloc(sizeof(char*) * (argc + 2));
		new_argv[0] = path;
		if (argv)	// Just to be safe, even tho argc == 0 if argv == null, still UB
			memcpy(&new_argv[1], argv, sizeof(char*) * argc);
		new_argv[argc + 1] = NULL;

		// we are the child, replace our process with the new one
		// if we could not replace our process then exit with that exit code (-1 probably, cast to char = 255)
		int code = execve(path, (char* const*)new_argv, (char* const*)envp);
		//printf("Failed to execve, %s\n", strerror(errno));
		exit(code);
	}
	return pid;
}


// if this returns 255 then we failed to run the program
int run_program(float timeout, const char* path, const char* const* argv, const char* const* envp)
{
	pid_t pid = run_program_pid(timeout, path, argv, envp);
	int status;
	waitpid(pid, &status, 0);
	return get_exit_code(status);
}
