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

static int	run_all(t_list *execs)
{
	size_t	i;
	int		wstatus;
	int		exit_status;
	pid_t	pid;

	i = 0;
	exit_status = 0;
	while (i < execs->count)
	{
		pid = executable_run(list_index_unchecked(execs, i));
		if (waitpid(pid, &wstatus, 0) == -1)
			exit_with_error("waitpid failed");
		if (WIFEXITED(wstatus))
		{
			exit_status = WEXITSTATUS(wstatus);
			if (exit_status)
				break ;
		}
		i++;
	}
	list_foreach(execs, (t_foreach_value)executable_un_init);
	return (exit_status);
}

// TODO: better error when invalid grammar rule
int	run_command_as_executable(const t_list *cmd)
{
	int				exit_status;
	t_list			execs;

	if (command_contains_invalid_grammar_rule(cmd))
		exit_with_error("Invalid redirect or pipe\n");
	list_init_safe(&execs, sizeof(t_executable));
	push_execs(&execs, cmd);
	exit_status = run_all(&execs);
	list_un_init(&execs, NULL);
	return (exit_status);
}
