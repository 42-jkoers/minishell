#include "command_read.h"
#include <stdlib.h>

static void	del(t_block *block)
{
	free(block->text);
}

void	free_blocks(t_list *blocks)
{
	list_un_init(blocks, (t_foreach_value)del);
}
