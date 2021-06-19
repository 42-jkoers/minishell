#include "ft_list.h"

void	list_foreach(t_list *list, const t_foreach_value foreach_value)
{
	size_t	i;

	i = 0;
	while (i < list->count)
	{
		foreach_value(list_index_unchecked(list, i));
		i++;
	}
}
