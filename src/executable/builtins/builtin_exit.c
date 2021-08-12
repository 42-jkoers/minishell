#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include "t_exit_data.h"
#include "ft_parse_utils.h"
#include "executable.h"
#include <sys/wait.h>

// So exit does this:
// 	prints out "exit\n" into stderr
// 	if non-numeric first arg: print out "minishell: exit: [ARG]: numeric
//argument required\n" into stderr, exit 2
//	if too manny args: print out "minishell: exit: too many arguments\n" into
//stderr, exit 1
//	exit [n] (default 0)

void	*builtin_exit_main(const t_executable *command)
{
	t_exit_data	*exit_data;
	char		*first_arg;
	int			current;

	exit_data = malloc(sizeof(t_exit_data));
	*exit_data = (t_exit_data){.print = NULL, .exit_code = 0,
		.actually_exit = true};
	if (command->args.count > 1)
	{
		first_arg = *(char **)list_index(&command->args, 1);
		current = 0;
		if (!read_llong(first_arg, &current, &exit_data->exit_code)
			 || first_arg[current] != '\0')
		{
			exit_data->exit_code = 2;
			exit_data->print = ft_strjoin_va(3, "minishell: exit: ", first_arg,
					": numeric argument required\n");
		}
		else if (command->args.count > 2)
			*exit_data = (t_exit_data){.print = ft_strdup("minishell: exit: too"
					" many arguments\n"), .exit_code = 1, .actually_exit = false}
				;
	}
	return (exit_data);
}

void	builtin_exit_main_cleanup(t_exit_data *exit_data, pid_t child_pid)
{
	int	wstatus;

	if (exit_data->actually_exit)
	{
		waitpid(child_pid, &wstatus, 0);
		exit(exit_data->exit_code);
	}
}

void	builtin_exit_child(t_exit_data *exit_data)
{
	if (*is_executing_command())
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (exit_data->print != NULL)
		ft_putstr_fd(exit_data->print, STDERR_FILENO);
	exit(exit_data->exit_code);
}
