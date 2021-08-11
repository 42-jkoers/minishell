#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>

// TODO check for invalid cmd_split
bool	cmd_to_blocks(t_list *blocks, const t_list *cmd_split)
{
	size_t	i;
	char	*current;

	i = 0;
	while (i < cmd_split->count)
	{
		current = list_index_unchecked(cmd_split, i);
		expand_environment_variables(&current);
		remove_quotes(&current);
		i++;
	}
	return (true);
}
