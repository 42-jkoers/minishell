#include "minishell.h"
#include "env.h"

#include <unistd.h>
#include <stdlib.h>
#include "malloc_wrappers.h"

const char	*working_directory_get(void)
{
	static char		*buff = NULL;
	static size_t	buff_size = 16;

	if (buff == NULL)
		buff = ft_malloc(buff_size * sizeof(char));
	while (!getcwd(buff, buff_size))
	{
		free(buff);
		buff_size *= 2;
		buff = ft_malloc(buff_size * sizeof(char));
	}
	return (buff);
}

// Not sure if i should also set the env, but hey, why not? :P

bool	working_directory_set(const char *path)
{
	int	result;

	result = chdir(path);
	if (result == -1)
		return (false);
	env_set("PWD", working_directory_get());
	return (true);
}
