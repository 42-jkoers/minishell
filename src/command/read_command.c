#include "minishell.h"
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
	invalid_command();
	return (NULL);
}

static t_list	split_command(const char *cmd)
{
	char	*prev_space;
	char	*current;
	t_list	blocks;

	if (!list_init(&blocks, sizeof(char *)))
		internal_error();
	prev_space = (char *)cmd - 1;
	current = (char *)cmd;
	while (*current)
	{
		if (*current != '\\' && current[1] == '"')
			current = jump_to_closing_quote(current);
		else if (*current == ' ')
		{
			if (current - prev_space > 1
				&& !list_push(&blocks,
					ft_strndup_unsafe(prev_space + 1, current - prev_space)))
				{
					printf("%s\n", ft_strndup_unsafe(prev_space + 1, 1));
					internal_error();
				}
			prev_space = current;
		}
		current++;
	}
	return (blocks);
}

void	test_print(char **str)
{
	printf("%s\n", *str);
}

t_list	read_command(void)
{
	const char		*cmd = readline("minishell$ ");
	const t_list	cmd_split = split_command(cmd);


	list_foreach(&cmd_split, test_print);
	return (cmd_split);
}
