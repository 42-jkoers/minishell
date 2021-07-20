#include <stdlib.h>
#include <unistd.h>
#include "find_executable.h"
#include "libft.h"
#include "env.h"
#include "utils.h"

int	main(int argc, char** argv, const char** envp)
{
	env_copy_ptr(envp);

	char	*path;

	if (argc != 2)
	{
		stupid_write(STDOUT_FILENO, "Usage: ./test_find_executable [exec_name]\n", 42);
		return (1);
	}
	path = find_executable_path(argv[1]);
	if (!path)
	{
		stupid_write(STDOUT_FILENO, "Error!\nFailed to find executable!\n", 34);
		return (1);
	}
	stupid_write(STDOUT_FILENO, "Found executable at: ", 21);
	stupid_write(STDOUT_FILENO, path, ft_strlen(path));
	stupid_write(STDOUT_FILENO, "\n", 1);
	free(path);
}
