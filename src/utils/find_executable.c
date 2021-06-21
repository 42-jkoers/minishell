#include <stdlib.h>
#include "libft.h"
#include <sys/types.h>
#include <dirent.h>

static char	*find_executable(char *name, char *path)
{
	DIR				*dir;
	char			*result;
	struct dirent	*dirent;

	dir = opendir(path);
	if (!dir)
		return (NULL);
	result = NULL;
	while (true)
	{
		dirent = readdir(dir);
		if (!dirent)
			break ;
		if (dirent->d_type != DT_REG)
			continue ;
		if (ft_strncmp(name, dirent->d_name, ~0) == 0)
			return (ft_strjoin_va(3, path, "/", name));
	}
	closedir(dir);
	return (result);
}

static void	free_split(char **split)
{
	size_t	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*find_executable_path(char *name)
{
	size_t	i;
	char	*path_env;
	char	**split;
	char	*result;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	split = ft_split(path_env, ':');
	if (!split)
		return (NULL);
	i = 0;
	result = NULL;
	while (split[i])
	{
		result = find_executable(name, split[i]);
		if (result)
			break ;
		i++;
	}
	free_split(split);
	return (result);
}
