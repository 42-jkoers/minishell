#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "command_read.h"
#include "malloc_wrappers.h"
#include "libft.h"

t_command_type	identify_command(const t_list *cmd)
{
	size_t	i;

	i = 0;
	while (i < cmd->count)
	{
		if (ft_strchr(*(char **)list_index(cmd, i), '='))
			return (C_ENV);
		i++;
	}
	return (C_EXECTUTABLE);
}
