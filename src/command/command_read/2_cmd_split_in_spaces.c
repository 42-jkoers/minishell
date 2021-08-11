#include "command_read.h"
#include "minishell.h"
#include <stdlib.h>
#include "malloc_wrappers.h"

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
	*end = *start;
	while (**end)
	{
		if (type_quote(**end))
			return ;
		if (ft_isspace(**end) || (get_grammar_rule_info(*end)).type)
			return ;
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
	if (type_quote(**start))
	{
		*end = ft_strchr(*start + 1, **start);
		if (*end)
			(*end)++;
		if (!(*end))
			return (FAIL);
	}
	else if (get_grammar_rule_info(*start).type)
	{
		*end = *start + get_grammar_rule_info(*start).len;
		return (SUCCESS);
	}
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

	list_init_safe(&split, sizeof(char *));
	start = (char *)cmd;
	while (true)
	{
		status = goto_next_split(&start, &end);
		if (status == FAIL)
		{
			list_free(&split, free);
			return (false);
		}
		list_push_safe(&split, ft_strndup_unsafe(start, end - start));
		if (status == DONE)
			return (true);
		start = end;
	}
}
