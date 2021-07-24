#include "command_read.h"
#include "minishell.h"

static t_blocktype	set_start(char **start)
{
	while (**start)
	{
		if (type_quote(**start))
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

static t_blocktype	set_end_env_declaration(
	char **current, char **start, char **end)
{
	char		*current2;

	current2 = *start;
	while (!ft_isspace(*current2) && *current2 != '=' && *current2)
		current2++;
	if (*current2 != '=')
		return (0);
	if (type_quote(current2[1]))
		return (handle_quoted_block(
				current, current2 + 2, end, B_ENV_DECLARATION));
	while (!ft_isspace(*current2) && *current2)
		current2++;
	*end = current2;
	*current = current2;
	return (B_ENV_DECLARATION);
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
		return (handle_quoted_block(current, *start, end, blocktype));
	blocktype = set_end_env_declaration(current, start, end);
	if (blocktype)
		return (blocktype);
	blocktype = set_end(current, start, end);
	return (blocktype);
}
