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

void	list_init_cap_safe(t_list *list, size_t data_size, size_t cap)
{
	if (!list_init_cap(list, data_size, cap))
		exit_with_error("malloc");
}

void	list_insert_safe(t_list *list, size_t index, const void *value)
{
	if (!list_insert(list, index, value))
		exit_with_error("malloc");
}

void	list_remove_safe(t_list *list, size_t index)
{
	if (!list_remove(list, index))
		exit_with_error("malloc");
}
