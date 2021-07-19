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
	block.text = ft_strndup_unsafe(start, len);
	block.type = blocktype;
	if (blocktype != B_SINGLE_QUOTE)
		expand_environment_variables(&block.text);
	if (!block.text || !block.text[0])
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

	list_init_safe(&blocks, sizeof(t_block));
	current = (char *)cmd;
	while (true)
	{
		blocktype = goto_next_split(&current, &start, &end);
		if (blocktype & B_ERROR)
		{
			command_read_destroy(&blocks);
			blocks.count = 0;
			return (blocks);
		}
		if (blocktype == B_END)
			break ;
		push_block(start, end - start, &blocks, blocktype);
	}
	if (blocks.count == 0)
		command_read_destroy(&blocks);
	return (blocks);
}

static bool	ends_with_pipe(const char *cmd)
{
	size_t	i;

	i = ft_strlen(cmd);
	while (i)
	{
		i--;
		if (!ft_isspace(cmd[i]))
			return (cmd[i] == '|');
	}
	return (false);
}

static void	handle_trailing_pipe(char **cmd)
{
	char	*pipe_read;
	char	*old;

	if (!ends_with_pipe(*cmd))
		return ;
	while (true)
	{
		pipe_read = readline("> ");
		old = *cmd;
		*cmd = ft_strjoin(*cmd, pipe_read);
		free(old);
		free(pipe_read);
		if (!ends_with_pipe(pipe_read))
			break ;
	}
}

// @description	read non-empty command from user
// @return		malloced char* array with command split in spaces according to
// 				bash
t_list	command_read(void)
{
	char	*cmd;
	t_list	cmd_split;

	while (true)
	{
		cmd = readline("minishell$ ");
		if (!cmd)
			exit(0);
		handle_trailing_pipe(&cmd);
		cmd_split = get_cmd_split(cmd);
		if (cmd_split.count > 0)
			add_history(cmd);
		free(cmd);
		if (cmd_split.count > 0)
			break ;
	}
	return (cmd_split);
}
