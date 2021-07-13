#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include "ft_ternary.h"

static t_blocktype	set_start(char **start)
{
	while (**start)
	{
		if (**start == '"')
		{
			(*start)++;
			return (B_DOUBLE_QUOTE);
		}
		if (**start == '\'')
		{
			(*start)++;
			return (B_SINGLE_QUOTE);
		}
		if (!ft_isspace(**start))
			return (B_NORMAL);
		(*start)++;
	}
	return (B_END);
}

// WARNING: It is important that the longest grammar rule comes first in the
// array. Eg: "echo "a" >> x" matches ">" as seperator, when it should
// match ">>"
t_grammarinfo	get_block_info(const char *str)
{
	if (!ft_strncmp(str, ">>", 2))
		return ((t_grammarinfo){B_DOUBLE_GREATER, 2});
	if (!ft_strncmp(str, "<<", 2))
		return ((t_grammarinfo){B_DOUBLE_LESSER, 2});
	if (!ft_strncmp(str, ">", 1))
		return ((t_grammarinfo){B_GREATER, 1});
	if (!ft_strncmp(str, "<", 1))
		return ((t_grammarinfo){B_LESSER, 1});
	if (!ft_strncmp(str, "|", 1))
		return ((t_grammarinfo){B_PIPE, 1});
	return ((t_grammarinfo){B_TEXT, 0});
}

static t_blocktype	set_end(char **current, char **start, char **end)
{
	if (!**start)
		return (B_END);
	*end = *start + 1;
	while (**end)
	{
		if ((get_block_info(*end)).type & B_GRAMMAR_RULE)
		{
			*current = *end;
			return (B_NORMAL);
		}
		if (ft_isspace(**end))
		{
			*current = *end + 1;
			return (B_NORMAL);
		}
		(*end)++;
	}
	*current = *end;
	return (B_NORMAL);
}

// @param **current		start reading from
// @param **start		start of found block
// @param **end			last char of block (exclusive)
t_blocktype	goto_next_split(char **current, char **start, char **end)
{
	t_blocktype		blocktype;
	t_grammarinfo	properies;

	if (!**current)
		return (B_END);
	*start = *current;
	properies = get_block_info(*start);
	if (properies.type & B_GRAMMAR_RULE)
	{
		*end = *current + properies.len;
		*current = *current + properies.len;
		return (properies.type);
	}
	blocktype = set_start(start);
	if (blocktype & (B_ERROR | B_END))
		return (blocktype);
	if (blocktype == B_SINGLE_QUOTE || blocktype == B_DOUBLE_QUOTE)
		return (handle_quoted_block(current, start, end, blocktype));
	blocktype = set_end(current, start, end);
	return (blocktype);
}
