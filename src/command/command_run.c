#include "t_command.h"
#include "t_fd_override.h"
#include "find_executable.h"
#include "env.h"

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

static char	**convert_args_list(const t_list *args)
{
	size_t	i;
	char	**ret;

	ret = malloc((args->count + 1) * sizeof(char *));
	if (ret == NULL)
		exit(1);
	i = 0;
	while (i < args->count)
	{
		ret[i] = *((char **)list_index(args, i));
		i++;
	}
	ret[args->count] = NULL;
	return (ret);
}

static void	apply_fd_overrides(const t_list *fd_overrides)
{
	size_t			i;
	t_fd_override	*override;

	i = 0;
	while (i < fd_overrides->count)
	{
		override = list_index(fd_overrides, i);
		dup2(override->new_fd, override->override_fd);
		i++;
	}
}

static void	close_fd(int *fd)
{
	close(*fd);
}

// TODO: buildins

pid_t	command_run(const t_command *command)
{
	pid_t	pid;

	pid = fork();
	if (pid != 0)
	{
		list_foreach(&command->main_close_fds, (const t_foreach_value)close_fd);
		return (pid);
	}
	apply_fd_overrides(&command->fd_overrides);
	list_foreach(&command->child_close_fds, (const t_foreach_value)close_fd);
	execve(command->executable_path, convert_args_list(&command->args),
		*env_ptr());
	exit(-1);
}
