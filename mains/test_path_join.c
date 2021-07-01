#include <stdlib.h>
#include <unistd.h>
#include "find_executable.h"
#include "libft.h"
#include "env.h"
#include "utils.h"
#include <stdio.h>

int	main(int argc, char** argv, const char** envp)
{
	env_copy_ptr(envp);

	if (argc != 3)
	{
		write(STDOUT_FILENO, "Usage: ./test_path_join path1 path2\n", 39);
		return (1);
	}

	else
	{
		char* new = path_join(argv[1], argv[2]);
		printf("Joined path: %s\n", new);
		free(new);
	}
}
