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

typedef enum e_startstatus
{
	NOTFOUND,
	FOUND_START,
	FOUND_QUOTED_TEXT,
}			t_startstatus;

static t_startstatus	set_start(char **start)
{
	bool	escaped;

	escaped = false;
	while (**start)
	{
		if (!escaped && **start == '"')
		{
			(*start)++;
			return (FOUND_QUOTED_TEXT);
		}
		if (**start != ' ')
		{
			return (FOUND_START);
		}
		escaped = !escaped && **start == '\\';
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
	{">>", ">>", ">", "<", "|"};
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

static bool	set_end_and_current(char **current, char **start, char **end)
{
	bool	escaped;

	if (!**start)
		return (false);
	*end = *start + 1;
	escaped = **start == '\\';
	while (**end)
	{
		if (!escaped && is_grammar_rule(*end))
			break ;
		if (!escaped && **end == ' ')
		{
			*current = *end + 1;
			return (true);
		}
		escaped = !escaped && **end == '\\';
		(*end)++;
	}
	*current = *end;
	return (true);
}

// @param **current		start reading from
// @param **start		start of found block
// @param **end			last char of block (exclusive)
bool	goto_next_split(char **current, char **start, char **end)
{
	t_startstatus	startstatus;
	size_t			grammar_rule_len;

	if (!**current)
		return (false);
	*start = *current;
	grammar_rule_len = is_grammar_rule(*current);
	if (grammar_rule_len > 0)
	{
		*end = *current + grammar_rule_len;
		*current = *current + grammar_rule_len;
		return (true);
	}
	startstatus = set_start(start);
	if (startstatus == NOTFOUND)
		return (false);
	if (startstatus == FOUND_QUOTED_TEXT)
	{
		*end = find_closing_quote(*start - 1);
		*current = *end + 1;
		return (true);
	}
	else
		return (set_end_and_current(current, start, end));
}
