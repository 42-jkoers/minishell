#include "executable.h"
#include "builtin.h"
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

pid_t	executable_run(const t_executable *executable)
{
	void					*builtin_data;
	const t_builtin_exec	*exec_data;
	pid_t					pid;

	exec_data = executable_get_builtin_exec(executable);
	if (exec_data != NULL)
		builtin_data = exec_data->main_func(executable);
	pid = fork();
	if (pid != 0)
	{
		if (exec_data != NULL)
			exec_data->main_cleanup_func(builtin_data);
		list_foreach(&executable->main_close_fds, (const t_foreach_value)
			close_fd);
		return (pid);
	}
	apply_fd_overrides(&executable->fd_overrides);
	list_foreach(&executable->child_close_fds, (const t_foreach_value)close_fd);
	if (exec_data != NULL)
		exec_data->child_func(builtin_data);
	else
		execve(executable->executable_path,
			convert_args_list(&executable->args), *env_ptr());
	exit(-1);
}