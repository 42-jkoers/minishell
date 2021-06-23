#include "libft.h"
#include "working_directory.h"

#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

// looks through a folder (at path) for a file named "name", if found, returns [path]/[name]
static char	*find_executable(char *name, char *path)
{
	DIR				*dir;
	struct dirent	*dirent;

	dir = opendir(path);
	if (!dir)
		return (NULL);
	while (true)
	{
		dirent = readdir(dir);
		if (!dirent)
			break ;
		if (dirent->d_type != DT_REG)
			continue ;
		if (ft_strncmp(name, dirent->d_name, ~0) == 0)
		{
			closedir(dir);
			return (ft_strjoin_va(3, path, "/", name));
		}
	}
	closedir(dir);
	return (NULL);
}

// Looks in the current work director for name, going through directories
// Im not too sure if open(path) is the best way to check if the file exists
// The access() function is better, but is not allowed in the subject... :(
static char* get_direct_path(char* name)
{
	char* path = ft_strjoin_va(3, working_directory_get(), "/", name);
	int fd = open(path, __O_PATH);
	if (fd != -1)
	{
		close(fd);
		return path;
	}
	free(path);
	return NULL;
}

char	*find_executable_path(char *name)
{
	size_t	i;
	char	*path_env;
	char	**split;
	char	*result;

	result = get_direct_path(name);
	if (result)
		return result;
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	split = ft_split(path_env, ':');
	if (!split)
		return (NULL);
	i = 0;
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
