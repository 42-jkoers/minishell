#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#include "catch_exceptions.h"
#include "pid_utils.h"
#include "JTester.h"

#include "test_utils.h"	// bad

bool WOULD_CATCH = false;

int catch_exceptions(float timeout, crash_func func, void* arg)
{
	pid_t pid = fork_with_timeout(timeout);
	if (pid == 0)
	{
		WOULD_CATCH = true;
		// we are the child, call the function and exit afterwards
		func(arg);
		exit(0);
	}
	int status;
	waitpid(pid, &status, 0);
	fflush(NULL);
	JTester_flush();	// Just for better feedback
	return get_exit_code(status);
}
