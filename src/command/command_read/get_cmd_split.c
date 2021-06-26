#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>

static char	*jump_to_matching_closing_quote(const char *str)
{
	char	*end;

	end = (char *)str + 1;
	while (*end)
	{
		if (*end != '\\' && end[1] == '"')
			return (end + 1);
		end++;
	}
	exit_with_error("Invalid command: missing closing quote");
	return (NULL);
}

bool	goto_next_split(char **start, char **end)
{
	while (ft_isspace(**start))
		(*start)++;
	if (!**start)
		return (false);
	if (**start == '\\' && (*start)[1] == '"')
	{
		*start = *start + 2;
		*end = jump_to_matching_closing_quote(*start);
		return (true);
	}
	*end = *start + 1;
	while (!ft_isspace(**end) && **end)
		(*end)++;
	return (true);
}

void	push_block(char *start, size_t len, t_list *blocks)
{
	char	*new;

	if (start[0] != '$')
	{
		list_push_safe(blocks, ft_strndup_unsafe(start, len));
		return ;
	}
	new = getenv(start + 1);
	if (new)
		list_push_safe(blocks, ft_strdup(new));
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
