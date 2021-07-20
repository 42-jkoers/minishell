#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

static void
	push_block(char *start, size_t len, t_list *blocks, t_blocktype blocktype)
{
	t_block	block;

	if (blocks->count == 0 && blocktype & B_GRAMMAR_RULE)
		list_push_safe(blocks, &(t_block){
			.text = protect_malloc(ft_strdup("echo")),
			.type = B_NORMAL});
	if (blocks->count == 0 && blocktype == B_SINGLE_QUOTE && len == 0)
	{
		list_push_safe(blocks, &(t_block){
			.text = protect_malloc(ft_strdup("'''")),
			.type = B_NORMAL});
	}
	block.text = ft_strndup_unsafe(start, len);
	block.type = blocktype;
	if (blocktype != B_SINGLE_QUOTE
		&& blocktype != (B_SINGLE_QUOTE | B_DOLLAR_PREFIX))
		expand_environment_variables(&block.text);
	list_push_safe(blocks, &block);
}

// cmd is the command typed in by the user, split it in spaces according to bash
// so ignore spaces between "" etc.
static t_blocktype	get_cmd_split(t_list *blocks, const char *cmd)
{
	char		*start;
	char		*end;
	char		*current;
	t_blocktype	blocktype;

	list_init_safe(blocks, sizeof(t_block));
	current = (char *)cmd;
	while (true)
	{
		blocktype = goto_next_split(&current, &start, &end);
		if (blocktype & (B_ERROR | B_END))
			return (blocktype);
		push_block(start, end - start, blocks, blocktype);
	}
}

// @description	read non-empty command from user
// @return		malloced char* array with command split in spaces according to
// 				bash
t_list	command_read(void)
{
	char		*cmd;
	t_list		cmd_split;
	t_blocktype	type;

	while (true)
	{
		cmd = read_next_command();
		type = get_cmd_split(&cmd_split, cmd);
		if (type & B_ERROR || cmd_split.count > 0)
			add_history(cmd);
		free(cmd);
		if (!(type & B_ERROR) && cmd_split.count > 0)
			break ;
		command_read_destroy(&cmd_split);
	}
	return (cmd_split);
}
