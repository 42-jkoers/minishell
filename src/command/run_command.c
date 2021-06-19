#include "t_command.h"
#include "t_fd_override.h"
#include "find_executable.h"

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

// Just changes a list<char*> to a null terminated char**

static char	**convert_args_list(t_list *args)
{
	size_t	i;
	char	**ret;

	ret = malloc((args->count + 1) * sizeof(char *));
	i = 0;
	while (i < args->count)
	{
		ret[args->count] = list_index(args, i);
		i++;
	}
	ret[args->count] = NULL;
	return (ret);
}

static void	apply_fd_overrides(t_list *fd_overrides)
{
	size_t			i;
	t_fd_override	*override;

	i = 0;
	while (i < fd_overrides->count)
	{
		override = list_index(&fd_overrides, i);
		dup2(override->new_fd, override->override_fd);
		i++;
	}
}

static void	close_fd(int *fd)
{
	close(*fd);
}

// TODO: buildins and use modified env

pid_t	run_command(const t_command *command)
{
	pid_t	pid;
	int		err;

	pid = fork();
	if (pid != 0)
		return (pid);
	apply_fd_overrides(&command->fd_overrides);
	list_foreach(&command->child_close_fds, close_fd);
	err = execve(command->executable_path, convert_args_list(&command->args),
			NULL);
	exit(-1);
}
