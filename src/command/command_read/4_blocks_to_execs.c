#include <unistd.h>

#include "minishell.h"
#include "executable.h"
#include "command_read.h"
#include "malloc_wrappers.h"

typedef struct s_redirect
{
	int						base_fd;
	t_file_redirect_mode	mode;
}		t_redirect;

static void	push_redirect(
	t_list *execs, const t_list *blocks, t_blocktype type, size_t *offset)
{
	t_executable	*left;
	t_redirect		r;
	char			*path;

	path = ((t_block *)list_index_unchecked(blocks, *offset + 1))->text;
	left = list_index_unchecked(execs, execs->count - 1);
	*offset += 2;
	if (type == B_DOUBLE_LESSER)
	{
		executable_add_here_doc(left, path);
		return ;
	}
	if (type == B_DOUBLE_GREATER)
		r = (t_redirect){STDOUT_FILENO, r_write | r_append};
	else if (type == B_GREATER)
		r = (t_redirect){STDOUT_FILENO, r_write};
	else if (type == B_LESSER)
		r = (t_redirect){STDIN_FILENO, r_read};
	else
		exit_with_error("Bad type");
	if (!executable_add_fd_file_redirect(left, r.base_fd, path, r.mode))
		exit_with_error("File could not be opened");
}

static void	push_exec(t_list *execs, const t_list *blocks, size_t *offset)
{
	t_executable	exec;
	t_block			*block;

	block = list_index_unchecked(blocks, *offset);
	executable_init(&exec, block->text);
	(*offset)++;
	while (*offset < blocks->count)
	{
		block = list_index_unchecked(blocks, *offset);
		if (block->type & B_GRAMMAR_RULE)
			break ;
		executable_add_arg(&exec, block->text);
		(*offset)++;
	}
	list_push_safe(execs, &exec);
}

static void	push_pipe_block(t_list *execs, const t_list *blocks, size_t *offset)
{
	t_executable	*left;
	t_executable	*right;

	(*offset)++;
	push_exec(execs, blocks, offset);
	left = list_index_unchecked(execs, execs->count - 2);
	right = list_index_unchecked(execs, execs->count - 1);
	executable_add_pipe(left, right);
}

// parse cmd into execs
bool	blocks_to_execs(t_list *execs, const t_list *blocks)
{
	t_block	*block;
	size_t	offset;

	offset = 0;
	list_init_safe(execs, sizeof(t_executable));
	push_exec(execs, blocks, &offset);
	while (offset < blocks->count)
	{
		block = list_index_unchecked(blocks, offset);
		if (block->type == B_PIPE)
			push_pipe_block(execs, blocks, &offset);
		else if (block->type & B_REDIRECT)
			push_redirect(execs, blocks, block->type, &offset);
	}
	return (true);
}
