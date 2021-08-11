#include "command_read.h"
#include "minishell.h"
#include <stdlib.h>
#include "malloc_wrappers.h"
#include "ft_ternary.h"

static t_status	set_start(char **start)
{
	if (!(**start))
		return (DONE);
	while (**start)
	{
		if (!ft_isspace(**start))
			return (SUCCESS);
		(*start)++;
	}
	return (FAIL);
}

static void	set_end(char **start, char **end)
{
	char	quote;

	*end = *start;
	quote = 0;
	while (**end)
	{
		if ((!quote && ft_isspace(**end)) || (get_grammar_rule_info(*end)).type)
			return ;
		if (type_quote(**end))
			quote = ter_char(**end == quote, 0, **start);
		(*end)++;
	}
}

// @param **start		start of found block
// @param **end			last char of block (exclusive)
t_status	goto_next_split(char **start, char **end)
{
	const t_status	status = set_start(start);

	if (status != SUCCESS)
		return (status);
	if (get_grammar_rule_info(*start).type)
		*end = *start + get_grammar_rule_info(*start).len;
	else
		set_end(start, end);
	return (SUCCESS);
}

// splits command in valid spaces
// `ls>> 'a b.txt'` --> [ls, >>, 'a b.txt']
bool	cmd_split_in_spaces(t_list *split, const char *cmd)
{
	char		*start;
	char		*end;
	t_status	status;

	list_init_safe(split, sizeof(char *));
	start = (char *)cmd;
	while (true)
	{
		status = goto_next_split(&start, &end);
		if (status == FAIL)
		{
			list_free(split, free);
			return (false);
		}
		if (status == DONE)
			return (true);
		list_push_safe(split, ft_strndup_unsafe(start, end - start));
		start = end;
	}
}
