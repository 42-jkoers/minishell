#include <sys/wait.h>

#include "minishell.h"
#include "executable.h"
#include "command_read.h"
#include "malloc_wrappers.h"

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
	exit_status = executable_run_all(&execs);
	list_un_init(&execs, (t_foreach_value)executable_un_init);
	return (exit_status);
}
