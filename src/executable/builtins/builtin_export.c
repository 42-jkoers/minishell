#include "t_executable.h"
#include "utils.h"
#include "libft.h"
#include "env.h"
#include <unistd.h>
#include <stdlib.h>

// the simpelest escape, just put it in '' and escape ' with \'

static void	log_env_data(char **variable)
{
	int		i;
	char	*eq;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	eq = ft_strchr(*variable, '=');
	if (!eq)
		ft_putstr_fd(*variable, STDOUT_FILENO);
	else
	{
		if (write(STDOUT_FILENO, *variable, (eq - *variable) / sizeof(char)
				 + sizeof(char)) == -1)
			 return ;
		ft_putchar_fd('\'', STDOUT_FILENO);
		i = 1;
		while (eq[i])
		{
			if (eq[i] == '\'' || eq[i] == '\\')
				ft_putchar_fd('\\', STDOUT_FILENO);
			ft_putchar_fd(eq[i], STDOUT_FILENO);
			i++;
		}
		ft_putchar_fd('\'', STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}

static void	add_arg(char **variable)
{
	char	*eq;
	char	*name;

	eq = ft_strchr(*variable, '=');
	if (!eq)
		return ;
	name = ft_substr(*variable, 0, eq - *variable);
	if (is_valid_env_string(name))
		env_set(name, eq + 1);
	else
	{
		ft_putstr_fd("minishell: export: '", STDERR_FILENO);
		ft_putstr_fd(name, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier", STDERR_FILENO);
	}
	free(name);
}

void	*builtin_export_main(const t_executable *command)
{
	if (command->args.count == 1)
		return ((void *)1);
	list_foreach_range(&command->args, (t_range){.start = 1, .end = command
		->args.count}, (t_foreach_value)add_arg);
	return ((void *)0);
}

void	builtin_export_main_cleanup(char *str, pid_t child_pid)
{
	(void)str;
	(void)child_pid;
}

void	builtin_export_child(bool *log_env)
{
	if (log_env)
		list_foreach_range(env_ptr(), (t_range){.start = 0, .end = env_ptr()
			->count - 1}, (t_foreach_value)log_env_data);
	exit(0);
}
