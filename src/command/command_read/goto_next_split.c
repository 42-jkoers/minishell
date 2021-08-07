#include "command_read.h"
#include "minishell.h"

static t_blocktype	set_start(char **start)
{
	while (**start)
	{
		if (type_quote(**start))
			return (type_quote(**start));
		if (!ft_isspace(**start))
			return (B_NORMAL);
		(*start)++;
	}
	return (B_END);
}

static t_blocktype	set_end(char **start, char **end)
{
	const t_grammarinfo	info = get_grammar_rule_info(*start);

	if (info.type & B_GRAMMAR_RULE)
	{
		*end = *start + info.len;
		return (info.type);
	}
	*end = *start;
	while (**end)
	{
		if (ft_isspace(**end)
			|| type_quote(**end)
			|| (get_grammar_rule_info(*end)).type)
			break ;
		(*end)++;
	}
	return (B_NORMAL);
}

// @param **current		start reading from
// @param **start		start of found block
// @param **end			last char of block (exclusive)
t_blocktype	goto_next_split(char **start, char **end)
{
	t_blocktype		blocktype;

	blocktype = set_start(start);
	if (blocktype & (B_ERROR | B_END))
		return (blocktype);
	if (blocktype & B_QUOTED)
		return (handle_quoted_block(*start, end, blocktype));
	blocktype = set_end(start, end);
	return (blocktype);
}
