#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#define EVALUATE_ESCAPED_CHARS 0

static void	escaped_to_ascii(char *c)
{
	const char	typed[] =
	{'a', 'b', 'e', 'f', 'n', 'r', 't', 'v', '?', };
	const char	escaped[] =
	{'\a', '\b', '\e', '\f', '\n', '\r', '\t', '\v', '\?', };
	size_t		i;

	i = 0;
	while (i < sizeof(typed) / sizeof(typed[0]))
	{
		if (*c == typed[i])
		{
			*c = escaped[i];
			return ;
		}
		i++;
	}
}

// eg. "a\ aa" --> "a aa"
//     "\\a" --> "\a"
static void	remove_escape_characters(char *cmd)
{
	size_t	i;
	bool	escaped;
	size_t	len;

	len = ft_strlen(cmd);
	escaped = false;
	i = 0;
	while (i < len)
	{
		if (!escaped && cmd[i] == '\\')
		{
			if (EVALUATE_ESCAPED_CHARS)
				escaped_to_ascii(cmd + i + 1);
			ft_memmove(cmd + i, cmd + i + 1, len - i);
			len -= 1;
		}
		escaped = !escaped && cmd[i] == '\\';
		i++;
	}
}

static void
	push_block(char *start, size_t len, t_list *blocks, t_blocktype blocktype)
{
	char	*block;

	block = ft_strndup_unsafe(start, len);
	if (blocktype == NORMAL)
		remove_escape_characters(block);
	if (blocktype != SINGLE_QUOTE)
		expand_environment_variables(&block);
	if (!block || !block[0])
		return ;
	list_push_safe(blocks, &block);
}

// cmd is the command typed in by the user, split it in spaces according to bash
// so ignore spaces between "" etc.
static t_list	get_cmd_split(const char *cmd)
{
	char		*start;
	char		*end;
	char		*current;
	t_list		blocks;
	t_blocktype	blocktype;

	list_init_safe(&blocks, sizeof(char *));
	current = (char *)cmd;
	while (true)
	{
		blocktype = goto_next_split(&current, &start, &end);
		if (blocktype == NOTFOUND)
			break ;
		push_block(start, end - start, &blocks, blocktype);
	}
	return (blocks);
}

// read command from user
// returns malloced char* array with command split in spaces according to bash
t_list	command_read(void)
{
	const char		*cmd = readline("minishell$ ");
	t_list			cmd_split;

	add_history(cmd);
	cmd_split = get_cmd_split(cmd);
	free(cmd);
	return (cmd_split);
}
