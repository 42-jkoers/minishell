#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include "ft_ternary.h"

static t_blocktype	set_start(char **start)
{
	bool	escaped;

	escaped = false;
	while (**start)
	{
		if (!escaped && **start == '"')
		{
			(*start)++;
			return (DOUBLE_QUOTE);
		}
		if (!escaped && **start == '\'')
		{
			(*start)++;
			return (SINGLE_QUOTE);
		}
		if (!escaped && **start != ' ')
			return (NORMAL);
		escaped = DO_ESCAPE && !escaped && **start == '\\';
		(*start)++;
	}
	return (NOTFOUND);
}

// @return	length of the matched grammer rule, 0 if not a grammer rule
// match >, >>, | etc
// WARNING: when updating grammar_rules, also update grammer_rule_len
// WARNINC: It is important that the longest grammar rule comes first in the
//          array. Eg: "echo "a" >> x" matches ">" as seperator, when it should
//          match ">>"
static size_t	is_grammar_rule(const char *str)
{
	const char		*grammar_rules[] =
	{">>", "<<", ">", "<", "|"};
	const size_t	grammer_rule_len[] =
	{2, 2, 1, 1, 1};
	size_t			i;

	i = 0;
	while (i < sizeof(grammar_rules) / sizeof(grammar_rules[0]))
	{
		if (!ft_strncmp(str, grammar_rules[i], grammer_rule_len[i]))
			return (grammer_rule_len[i]);
		i++;
	}
	return (0);
}

static t_blocktype	set_end(char **current, char **start, char **end)
{
	bool	escaped;

	if (!**start)
		return (NOTFOUND);
	*end = *start + 1;
	escaped = DO_ESCAPE && **start == '\\';
	while (**end)
	{
		if (!escaped && is_grammar_rule(*end))
		{
			*current = *end;
			return (GRAMMAR_RULE);
		}
		if (!escaped && **end == ' ')
		{
			*current = *end + 1;
			return (NORMAL);
		}
		escaped = DO_ESCAPE && !escaped && **start == '\\';
		(*end)++;
	}
	*current = *end;
	return (NORMAL);
}

// @param **current		start reading from
// @param **start		start of found block
// @param **end			last char of block (exclusive)
t_blocktype	goto_next_split(char **current, char **start, char **end)
{
	t_blocktype		blocktype;
	size_t			grammar_rule_len;

	if (!**current)
		return (NOTFOUND);
	*start = *current;
	grammar_rule_len = is_grammar_rule(*current);
	if (grammar_rule_len > 0)
	{
		*end = *current + grammar_rule_len;
		*current = *current + grammar_rule_len;
		return (GRAMMAR_RULE);
	}
	blocktype = set_start(start);
	if (blocktype == NOTFOUND)
		return (NOTFOUND);
	if (blocktype == DOUBLE_QUOTE || blocktype == SINGLE_QUOTE)
		return (handle_quoted_block(current, start, end, blocktype));
	blocktype = set_end(current, start, end);
	return (blocktype);
}
