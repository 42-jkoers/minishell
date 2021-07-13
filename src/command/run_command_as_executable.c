#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "minishell.h"
#include "executable.h"
#include "command_read.h"
#include "malloc_wrappers.h"

// TODO: error when no right expression on grammar rule
// TODO: output redirect
t_executable
	*push_executables(t_list *executables, const t_list *cmd, size_t i)
{
	t_executable	exec;
	t_executable	*new;
	t_block			*block;

	executable_init(&exec, ((t_block *)list_index(cmd, i))->text);
	i++;
	while (i < cmd->count)
	{
		block = (t_block *)list_index_unchecked(cmd, i);
		if (block->type & B_GRAMMAR_RULE)
		{
			new = push_executables(executables, cmd, i + 1);
			if (block->type == B_PIPE)
				executable_add_pipe(&exec, new);
			break ;
		}
		else
			executable_add_arg(&exec, block->text);
		i++;
	}
	list_push_safe(executables, &exec);
	return (list_index_unchecked(executables, executables->count - 1));
}

static void	run_all(pid_t *pids, int *statuses, t_list *executables)
{
	size_t	i;

	i = executables->count;
	while (i)
	{
		i--;
		pids[i] = executable_run(list_index_unchecked(executables, i));
	}
	i = executables->count;
	while (i)
	{
		i--;
		waitpid(pids[i], &statuses[i], 0);
	}
	i = executables->count;
	while (i)
	{
		i--;
		executable_un_init(list_index_unchecked(executables, i));
	}
}

// TODO: do something with return value of waitpid?
// TODO: exit code??
int	run_command_as_executable(const t_list *cmd)
{
	pid_t			*pids;
	int				*statuses;
	t_list			executables;

	list_init_safe(&executables, sizeof(t_executable));
	push_executables(&executables, cmd, 0);
	pids = ft_malloc(executables.count * sizeof(pid_t));
	statuses = ft_malloc(executables.count * sizeof(int));
	run_all(pids, statuses, &executables);
	list_un_init(&executables, NULL);
	free(pids);
	free(statuses);
	return (0);
}
