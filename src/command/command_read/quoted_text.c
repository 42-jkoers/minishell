#include "command_read.h"
#include "minishell.h"
#include <stdio.h>

// assuming that **start is the starting quote
t_blocktype	handle_quoted_block(const char *start, char **end,
				t_blocktype blocktype)
{
	const char	quote = *start;

	*end = ft_strchr(start + 1, quote);
	if (!(*end))
	{
		printf("WARNING: no closing %c found\n", quote);
		return (B_NO_CLOSING_QUOTE);
	}
	(*end)++;
	return (blocktype);
}
