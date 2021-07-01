#include "t_executable.h"
#include "working_directory.h"
#include "utils.h"
#include "env.h"
#include "ft_list.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

static void parse_unset(char** ret, char** current)
{
	if (is_valid_env_string(*current))
		env_set(*current, NULL);
	else
	{
		char* prev = *ret;
		*ret = ft_strjoin_va(4, *ret, "minishell: unset: '", *current, "': not a valid identifier\n");
		if (prev)
			free(prev);
	}
}

void	*builtin_unset_main(const t_executable *command)
{
	char* ret;

	ret = NULL;
	list_foreach_range_data(&command->args, &ret, (t_range){.start = 1, .end = command->args.count}, (t_foreach_data_value)parse_unset);
	return (ret);
}

void	builtin_unset_main_cleanup(char *str, pid_t child_pid)
{
	(void)child_pid;
	if (str != NULL)
		free(str);
}

void	builtin_unset_child(char *str)
{
	if (str != NULL)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		exit(1);
	}
	exit(0);
}
