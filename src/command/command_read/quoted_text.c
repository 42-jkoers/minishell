#include "command_read.h"
#include "minishell.h"
#include <stdio.h>

// assuming that **start is first char after the starting quote
t_blocktype	handle_quoted_block(char **current, char **start, char **end,
				t_blocktype blocktype)
{
	const char	quote = *(*start - 1);

	*end = ft_strchr(*start, quote);
	if (!end)
	{
		printf("WARNING: no closing %c found\n", quote);
		return (B_NO_CLOSING_QUOTE);
	}
	*current = *end + 1;
	return (blocktype);
}
