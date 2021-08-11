#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>

// TODO check for invalid cmd_split
// TODO give correct type
bool	cmd_to_blocks(t_list *blocks, const t_list *cmd_split)
{
	size_t	i;
	t_block	block;

	list_init_safe(blocks, sizeof(t_block));
	i = 0;
	while (i < cmd_split->count)
	{
		block.text = mc(ft_strdup(list_index_unchecked(cmd_split, i)));
		expand_environment_variables(&block.text);
		remove_quotes(&block.text);
		block.type = B_NORMAL;
		list_push_safe(blocks, &block);
		i++;
	}
	return (true);
}
