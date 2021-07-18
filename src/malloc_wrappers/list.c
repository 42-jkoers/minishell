#include "minishell.h"

void	list_push_safe(t_list *list, const void *value)
{
	if (!list_push(list, value))
		exit_with_error("malloc");
}

void	list_init_safe(t_list *list, size_t data_size)
{
	if (!list_init(list, data_size))
		exit_with_error("malloc");
}
