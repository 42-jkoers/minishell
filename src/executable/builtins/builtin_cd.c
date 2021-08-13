#include "t_executable.h"
#include "working_directory.h"
#include "utils.h"
#include "env.h"
#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

static bool	is_absolute_path(const char *path)
{
	return (path[0] == '/');
}

// Handle absolute and relative paths
// Get target directory

void	*builtin_cd_main(const t_executable *command)
{
	char		*path;
	bool		success;
	const char	*dir;

	if (!(command->args.count >= 1 && command->args.count <= 2))
		return (ft_strdup(SHELL": cd: too many arguments\n"));
	if (command->args.count == 1)
		dir = env_get("HOME");
	else
		dir = *(const char **)list_index(&command->args, 1);
	if (is_absolute_path(dir))
		path = ft_strdup(dir);
	else
		path = path_join(working_directory_get(), dir);
	success = working_directory_set(path);
	free(path);
	if (!success)
		return (ft_strjoin_va(3, SHELL": cd: ", dir, ": No such file or dir"
				"ectory\n"));
	return (NULL);
}

void	builtin_cd_main_cleanup(char *str, pid_t child_pid)
{
	(void)child_pid;
	if (str != NULL)
		free(str);
}

void	builtin_cd_child(char *str)
{
	if (str != NULL)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		exit(1);
	}
	exit(0);
}
