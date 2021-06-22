#include "minishell.h"
#include "env.h"

#include <unistd.h>
#include <stdlib.h>

char	*working_directory_get(void)
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

void	working_directory_set(char *path)
{
	chdir(path);
	env_set("PWD", path);
}
