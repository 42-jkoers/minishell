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

static void	run_all(pid_t *pids, int *statuses, t_list *execs)
{
	size_t	i;

	i = 0;
	while (i < execs->count)
	{
		pids[i] = executable_run(list_index_unchecked(execs, i));
		i++;
	}
	i = 0;
	while (i < execs->count)
	{
		waitpid(pids[i], &statuses[i], 0);
		i++;
	}
	i = 0;
	while (i < execs->count)
	{
		executable_un_init(list_index_unchecked(execs, i));
		i++;
	}
}

// TODO: do something with return value of waitpid?
// TODO: exit code / return value?
// TODO: better error when invalid grammar rule
int	run_command_as_executable(const t_list *cmd)
{
	pid_t			*pids;
	int				*statuses;
	t_list			execs;

	if (command_contains_invalid_grammar_rule(cmd))
	{
		printf("Invalid redirect or pipe\n");
		return (1);
	}
	list_init_safe(&execs, sizeof(t_executable));
	push_execs(&execs, cmd);
	pids = ft_malloc(execs.count * sizeof(pid_t));
	statuses = ft_malloc(execs.count * sizeof(int));
	run_all(pids, statuses, &execs);
	free(pids);
	free(statuses);
	list_un_init(&execs, NULL);
	return (0);
}
