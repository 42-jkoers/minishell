#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "minishell.h"
#include "executable.h"
#include "read_and_execute.h"
#include "malloc_wrappers.h"
#include "ft_loop.h"

bool	*is_executing_command(void)
{
	static bool	is_executing_command = false;

	return (&is_executing_command);
}

static int	get_exit_code(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	else if (WIFSTOPPED(wstatus))
		return (WSTOPSIG(wstatus));
	return (-1);
}

static int	executable_raw_run_all(const t_list *execs)
{
	t_executable	*executable;
	pid_t			*pid;
	int				wstatus;
	t_list			pids;

	list_init_cap_safe(&pids, sizeof(pid_t), execs->count);
	executable = NULL;
	while (loop_l(execs, (void **)&executable))
		list_push_safe(&pids, &(pid_t){executable_run(executable)});
	pid = NULL;
	while (loop_l(&pids, (void **)&pid))
		waitpid(*pid, &wstatus, 0);
	list_un_init(&pids, NULL);
	return (get_exit_code(wstatus));
}

// Make sure to close all the fd's that would have been closed when executed
static void	close_main_fds(t_executable *executable)
{
	size_t	i;

	i = 0;
	while (i < executable->main_close_fds.count)
	{
		close(*(int *)list_index(&executable->main_close_fds, i));
		i++;
	}
}

int	run_execs(const t_list *execs)
{
	int		exit_code;
	pid_t	pid;
	int		wstatus;

	*is_executing_command() = true;
	if (execs->count <= 1)
	{
		exit_code = executable_raw_run_all(execs);
		*is_executing_command() = false;
		return (exit_code);
	}
	pid = fork();
	if (pid == 0)
	{
		*is_executing_command() = false;
		exit(executable_raw_run_all(execs));
	}
	list_foreach(execs, (t_foreach_value)close_main_fds);
	waitpid(pid, &wstatus, 0);
	*is_executing_command() = false;
	return (get_exit_code(wstatus));
}
