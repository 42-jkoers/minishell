#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>

static char	*find_closing_quote(const char *start, char closing_quote)
{
	char	*current;
	bool	escaped;

	escaped = false;
	current = (char *)start;
	while (*current)
	{
		if (!escaped && *current == closing_quote)
			return (current);
		escaped = !escaped && *current == '\\';
		current++;
	}
	return (NULL);
}

// assuming that **start is first char after the starting quote
t_blocktype	handle_quoted_block(char **current, char **start, char **end,
				t_blocktype blocktype)
{
	const char	quote = *(*start - 1);

	*end = find_closing_quote(*start, quote);
	if (!*end)
	{
		printf("WARNING: no closing %c found\n", quote);
		return (NO_CLOSING_QUOTE);
	}
	*current = *end + 1;
	return (blocktype);
}
