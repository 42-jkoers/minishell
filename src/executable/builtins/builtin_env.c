#include "libft.h"
#include "env.h"
#include <unistd.h>
#include <stdlib.h>

static void	log_env_data(char **variable)
{
	if (ft_strncmp(*variable, "?=", 2) == 0
		|| ft_strncmp(*variable, "0=", 2) == 0)
		return ;
	ft_putstr_fd(*variable, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	builtin_env_child(void *data)
{
	(void)data;
	list_foreach_range(env_ptr(), (t_range){.start = 0, .end = env_ptr()
		->count - 1}, (t_foreach_value)log_env_data);
	exit(0);
}
