#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#include "minishell.h"
#include "executable.h"
#include "command_read.h"
#include "malloc_wrappers.h"
#include "ft_list.h"

typedef struct s_redirect
{
	int						base_fd;
	t_file_redirect_mode	mode;
}		t_redirect;

static void	push_redirect(
	t_list *execs, const t_list *cmd, t_blocktype type, size_t *offset)
{
	t_executable	*left;
	t_redirect		r;
	char			*path;

	path = ((t_block *)list_index_unchecked(cmd, *offset + 1))->text;
	left = list_index_unchecked(execs, execs->count - 1);
	if (type == B_DOUBLE_GREATER)
		r = (t_redirect){STDOUT_FILENO, r_write | r_append};
	else if (type == B_GREATER)
		r = (t_redirect){STDOUT_FILENO, r_write};
	else if (type == B_DOUBLE_LESSER)
		r = (t_redirect){STDIN_FILENO, r_read};
	else if (type == B_GREATER)
		r = (t_redirect){STDIN_FILENO, r_read};
	if (!executable_add_fd_file_redirect(left, r.base_fd, path, r.mode))
		exit_with_error("File could not be opened");
	*offset += 2;
}

static void	push_exec(t_list *execs, const t_list *cmd, size_t *offset)
{
	t_executable	exec;
	t_block			*block;

	block = list_index_unchecked(cmd, *offset);
	executable_init(&exec, block->text);
	(*offset)++;
	block = list_index_unchecked(cmd, *offset);
	while (*offset < cmd->count && !(block->type & B_GRAMMAR_RULE))
	{
		block = list_index_unchecked(cmd, *offset);
		executable_add_arg(&exec, block->text);
		(*offset)++;
	}
	list_push_safe(execs, &exec);
}

static void	push_pipe_block(t_list *execs, const t_list *cmd, size_t *offset)
{
	t_executable	*left;
	t_executable	*right;

	(*offset)++;
	left = list_index_unchecked(execs, execs->count - 1);
	push_exec(execs, cmd, offset);
	right = list_index_unchecked(execs, execs->count - 1);
	executable_add_pipe(left, right);
}

bool	command_contains_invalid_grammar_rule(const t_list *cmd)
{
	size_t	i;
	t_block	*current;
	bool	prev_was_grammer_rule;

	if (cmd->count == 0)
		return (false);
	prev_was_grammer_rule = true;
	i = 0;
	while (i + 1 < cmd->count)
	{
		current = list_index_unchecked(cmd, i);
		if ((current->type & B_GRAMMAR_RULE) && prev_was_grammer_rule)
			return (true);
		prev_was_grammer_rule = current->type & B_GRAMMAR_RULE;
		i++;
	}
	current = list_index_unchecked(cmd, i);
	return (current->type & B_GRAMMAR_RULE);
}

void	push_execs(t_list *execs, const t_list *cmd)
{
	t_block			*block;
	size_t			offset;

	offset = 0;
	push_exec(execs, cmd, &offset);
	while (offset < cmd->count)
	{
		block = list_index_unchecked(cmd, offset);
		if (block->type == B_PIPE)
			push_pipe_block(execs, cmd, &offset);
		else if (block->type & B_REDIRECT)
			push_redirect(execs, cmd, block->type, &offset);
	}
}
