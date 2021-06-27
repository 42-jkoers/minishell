#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>

char	*find_closing_quote(const char *start)
{
	char	*current;
	bool	is_escaped;

	is_escaped = *start == '\\';
	current = (char *)start + 1;
	while (*current)
	{
		if (!is_escaped && *current == '"')
			return (current);
		is_escaped = *current == '\\';
		current++;
	}
	exit_with_error("Invalid command: missing closing quote");
	return (NULL);
}

bool	find_start(char **start, char **end, bool *is_quoted)
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
		escaped = **start == '\\';
		(*start)++;
	}
	return (false);
}

bool	find_end(char **start, char **end)
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
		escaped = **end == '\\';
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

void	push_block(char *start, size_t len, t_list *blocks)
{
	char	*block;

	if (start[0] == '$')
		block = getenv(start + 1);
	else
		block = ft_strndup_unsafe(start, len);
	if (block)
		list_push_safe(blocks, &block);
}

// cmd is the command typed in by the user, split it in spaces according to bash
// so ignore spaces between "" etc.
t_list	get_cmd_split(const char *cmd)
{
	char	*start;
	char	*end;
	t_list	blocks;

	list_init_safe(&blocks, sizeof(char *));
	start = (char *)cmd;
	while (goto_next_split(&start, &end))
	{
		push_block(start, end - start, &blocks);
		start = end;
	}
	return (blocks);
}
