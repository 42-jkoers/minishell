#include "command_read.h"
#include <stdlib.h>

static void	del(t_block *block)
{
	free(block->text);
}

void	command_read_destroy(t_list *cmd)
{
	list_un_init(cmd, (t_foreach_value)del);
}
