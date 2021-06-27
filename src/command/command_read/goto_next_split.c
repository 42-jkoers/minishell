#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>

static char	*find_closing_quote(const char *start)
{
	char	*current;
	bool	escaped;

	escaped = *start == '\\';
	current = (char *)start + 1;
	while (*current)
	{
		if (!escaped && *current == '"')
			return (current);
		escaped = !escaped && *current == '\\';
		current++;
	}
	exit_with_error("Invalid command: missing closing quote");
	return (NULL);
}

static bool	find_start(char **start, char **end, bool *is_quoted)
{
	bool	escaped;

	escaped = false;
	while (**start)
	{
		if (!escaped && **start == '"')
		{
			*is_quoted = true;
			*end = find_closing_quote(*start);
			(*start)++;
			return (true);
		}
		if (**start != ' ')
		{
			return (true);
		}
		escaped = !escaped && **start == '\\';
		(*start)++;
	}
	return (false);
}

static bool	find_end(char **start, char **end)
{
	bool	escaped;

	if (!**start)
		return (false);
	*end = *start + 1;
	escaped = **start == '\\';
	while (**end)
	{
		if (!escaped && **end == ' ')
			return (true);
		escaped = !escaped && **end == '\\';
		(*end)++;
	}
	return (true);
}

bool	goto_next_split(char **start, char **end)
{
	static bool	is_quoted = false;

	if (!**start)
		return (false);
	if (is_quoted)
	{
		(*start)++;
		is_quoted = false;
	}
	if (!find_start(start, end, &is_quoted))
		return (false);
	if (is_quoted)
		return (true);
	return (find_end(start, end));
}
