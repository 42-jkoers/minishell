#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static void
	push_block(char *start, size_t len, t_list *blocks, t_blocktype blocktype)
{
	char	*block;

	block = ft_strndup_unsafe(start, len);
	if (DO_ESCAPE && blocktype == NORMAL)
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
		if (blocktype == NO_CLOSING_QUOTE)
		{
			list_un_init_ptr(&blocks);
			blocks.count = 0;
			return (blocks);
		}
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
	char	*cmd;
	t_list	cmd_split;

	while (true)
	{
		cmd = readline("minishell$ ");
		if (!cmd)
			exit(0);
		add_history(cmd);
		cmd_split = get_cmd_split(cmd);
		free(cmd);
		if (cmd_split.count > 0)
			break ;
	}
	return (cmd_split);
}
