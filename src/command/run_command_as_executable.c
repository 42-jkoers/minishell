#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include "minishell.h"
#include "executable.h"
#include "command_read.h"
#include "malloc_wrappers.h"
#include "ft_list.h"
#include "builtin.h"

pid_t	*get_running_executable(void)
{
	static pid_t	pid = -1;

	return (&pid);
}

static int	run_all(t_list *execs)
{
	pid_t	*pid;
	size_t	i;
	int		wstatus;

	pid = get_running_executable();
	i = 0;
	while (i < execs->count)
	{
		*pid = executable_run(list_index_unchecked(execs, i));
		if (waitpid(*pid, &wstatus, 0) == -1)
			exit_with_error("waitpid failed");
		*pid = -1;
		if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus))
			return (WEXITSTATUS(wstatus));
		i++;
	}
	return (0);
}

// returns exit status of command
int	run_command_as_executable(const t_list *cmd)
{
	int				exit_status;
	t_list			execs;

	if (cmd->count == 0)
		return (0);
	if (handle_invalid_grammar_rule(cmd))
		return (2);
	list_init_safe(&execs, sizeof(t_executable));
	push_execs(&execs, cmd);
	exit_status = run_all(&execs);
	list_un_init(&execs, (t_foreach_value)executable_un_init);
	return (exit_status);
}
