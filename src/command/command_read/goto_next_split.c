#include "command_read.h"
#include "minishell.h"

static t_blocktype	type_quote(char c)
{
	if (c == '\'')
		return (B_SINGLE_QUOTE);
	if (c == '"')
		return (B_DOUBLE_QUOTE);
	return (B_ERROR);
}

static t_blocktype	set_start(char **start)
{
	while (**start)
	{
		if (type_quote(**start) & B_QUOTED)
		{
			(*start)++;
			return (type_quote((*start)[-1]));
		}
		if (**start == '$' && type_quote((*start)[1]))
		{
			(*start) += 2;
			return (type_quote((*start)[-1]) | B_DOLLAR_PREFIX);
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
t_grammarinfo	get_grammar_rule_info(const char *str)
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
	return ((t_grammarinfo){B_ERROR, 0});
}

static t_blocktype	set_end(char **current, char **start, char **end)
{
	t_grammarinfo	info;

	*end = *start;
	while (**end)
	{
		info = get_grammar_rule_info(*end);
		if (info.type & B_GRAMMAR_RULE)
		{
			if (*end == *start)
				*end += info.len;
			*current = *end;
			return (info.type);
		}
		if (ft_isspace(**end) || type_quote(**end) & B_QUOTED)
		{
			*current = *end;
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

	*start = *current;
	blocktype = set_start(start);
	if (blocktype & (B_ERROR | B_END))
		return (blocktype);
	if (blocktype & B_QUOTED)
		return (handle_quoted_block(current, start, end, blocktype));
	blocktype = set_end(current, start, end);
	return (blocktype);
}
