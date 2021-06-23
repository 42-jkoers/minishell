#include "command.h"
#include "working_directory.h"
#include "env.h"
#include "libft.h"
#include "find_executable.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void test_executable(char* executable_path)
{
	char* path = find_executable_path(executable_path);
	if (path == NULL)
		printf("Failed to find executable: %s\n", executable_path);
	free(path);
}

int	main(int argc, char** argv, char** envp)
{
	env_ptr_copy(envp);
	(void)argc;
	(void)argv;

	test_executable("test_pipe");
	test_executable("./test_pipe");

	// Change the work directory to the mains folder
	char* new_path = ft_strjoin(working_directory_get(), "/mains");
	working_directory_set(new_path);
	free(new_path);

	test_executable("../test_pipe");

	test_executable("ls");

	printf("Success! finding paths is working!\n");

	return 0;
}
