#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// returns true on success
static bool	expand_environment_variables_safe(t_block *block, t_list *blocks)
{
	t_block	*prev;
	char	*expanded;
	bool	ambiguous_redirect;

	if (!ft_strchr(block->text, '$'))
		return (true);
	expanded = expand_environment_variables(block->text);
	ambiguous_redirect = false;
	if (blocks->count > 0)
	{
		prev = list_index_unchecked(blocks, blocks->count - 1);
		ambiguous_redirect
			= prev->type & B_REDIRECT && ft_strchr(expanded, ' ');
		if (ambiguous_redirect)
			printf("minishell: %s: ambiguous redirect\n", block->text);
	}
	free(block->text);
	block->text = expanded;
	block->type |= B_CONTAINS_EXPANDED_ENV;
	return (!ambiguous_redirect);
}

void	clean_env_declaration(char *text)
{
	char	*equals;

	equals = ft_strchr(text, '=');
	if (!equals)
		return ;
	if (type_quote(equals[1]))
		ft_memmove(equals + 1, equals + 2, ft_strlen(equals + 2) + 1);
}

static bool
	push_block(char *start, size_t len, t_list *blocks, t_blocktype blocktype)
{
	t_block	block;

	if (blocks->count == 0 && blocktype & B_REDIRECT)
		list_push_safe(blocks, &(t_block){
			.text = protect_malloc(ft_strdup("echo")),
			.type = B_NORMAL});
	if (blocks->count == 0 && blocktype == B_SINGLE_QUOTE && len == 0)
	{
		list_push_safe(blocks, &(t_block){
			.text = protect_malloc(ft_strdup("'''")),
			.type = B_NORMAL});
		return (true);
	}
	block.text = ft_strndup_unsafe(start, len);
	block.type = blocktype;
	if (blocktype & B_ENV_DECLARATION)
		clean_env_declaration(block.text);
	if (!(blocktype & B_SINGLE_QUOTE_ONLY)
		&& !expand_environment_variables_safe(&block, blocks))
		return (false);
	if (block.text[0])
		list_push_safe(blocks, &block);
	return (true);
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
		if (!push_block(start, end - start, blocks, blocktype))
			return (B_ERROR);
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
