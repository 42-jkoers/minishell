#include <stdlib.h>
#include <unistd.h>
#include "find_executable.h"
#include "libft.h"
#include "env.h"

int	main(int argc, char** argv, const char** envp)
{
	env_ptr_copy(envp);

	char	*path;

	if (argc != 2)
	{
		write(STDOUT_FILENO, "Usage: ./test_find_executable [exec_name]\n", 42);
		return (1);
	}
	path = find_executable_path(argv[1]);
	if (!path)
	{
		write(STDOUT_FILENO, "Error!\nFailed to find executable!\n", 34);
		return (1);
	}
	write(STDOUT_FILENO, "Found executable at: ", 21);
	write(STDOUT_FILENO, path, ft_strlen(path));
	write(STDOUT_FILENO, "\n", 1);
	free(path);
}
