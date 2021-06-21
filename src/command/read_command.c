#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static char	*jump_to_closing_quote(const char *str)
{
	char	*current;

	current = (char *)str + 1;
	while (*current)
	{
		if (*current != '\\' && current[1] == '"')
			return (current + 1);
		current++;
	}
	exit_with_error("Invalid command: missing closing quote");
	return (NULL);
}

static t_list	split_command(const char *cmd)
{
	char	*prev_space;
	char	*current;
	t_list	blocks;

	list_init_safe(&blocks, sizeof(char *));
	prev_space = (char *)cmd - 1;
	current = (char *)cmd;
	while (*current)
	{
		if (*current != '\\' && current[1] == '"')
			current = jump_to_closing_quote(current);
		else if (*current == ' ' || *current == '\0')
		{
			if (current - prev_space > 1)
				list_push_safe(&blocks,
					ft_strndup_unsafe(prev_space + 1, current - prev_space));
			prev_space = current;
		}
		current++;
	}
	return (blocks);
}

// changing eg. $FOO to the value of $FOO
// TODO: actually make it do what it says
static void	evaluate_environment_variables(void *command)
{
}

// read command from user
t_list	read_command(void)
{
	const char		*cmd = readline("minishell$ ");
	const t_list	cmd_split = split_command(cmd);

	list_foreach(&cmd_split, evaluate_environment_variables);
	return (cmd_split);
}
