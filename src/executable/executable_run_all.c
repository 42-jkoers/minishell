#include <sys/wait.h>

#include "minishell.h"
#include "executable.h"
#include "command_read.h"
#include "malloc_wrappers.h"
#include "ft_loop.h"

static int	get_exit_code(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (WTERMSIG(wstatus));
	else if (WIFSTOPPED(wstatus))
		return (WSTOPSIG(wstatus));
	return (-1);
}

int	executable_run_all(t_list *executables)
{
	t_executable	*executable;
	pid_t			*pid;
	int				wstatus;
	t_list			pids;

	list_init_cap_safe(&pids, sizeof(pid_t), executables->count);
	executable = NULL;
	while (loop_l(executables, (void **)&executable))
		list_push_safe(&pids, &(pid_t){executable_run(executable)});
	pid = NULL;
	while (loop_l(&pids, (void **)&pid))
		waitpid(*pid, &wstatus, 0);
	list_un_init(&pids, NULL);
	return (get_exit_code(wstatus));
}
