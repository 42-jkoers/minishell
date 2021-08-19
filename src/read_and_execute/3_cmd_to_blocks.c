#include "read_and_execute.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include "ft_list.h"
#include "ft_ternary.h"

static t_blocktype	last_blocktype(const t_list *blocks)
{
	if (blocks->count == 0)
		return (0);
	return (((t_block *)list_index(blocks, blocks->count - 1))->type);
}

static bool	is_empty_string_as_excutable(
		const t_list *blocks, t_block *block, const char *text)
{
	if (blocks->count == 0
		&& (!ft_strcmp(text, "''") || !ft_strcmp(text, "\"\"")))
	{
		block->text = protect_malloc(ft_strdup("''"));
		block->type = B_TEXT;
		return (true);
	}
	return (false);
}

static bool	is_grammar_rule(t_block *block, const char *text)
{
	const t_grammarinfo	info = get_grammar_rule_info(text);

	if (!info.len)
		return (false);
	block->text = protect_malloc(ft_strdup(text));
	block->type = info.type;
	return (true);
}

static t_block	to_block(const t_list *blocks, const char *text)
{
	t_block	block;

	if (is_empty_string_as_excutable(blocks, &block, text))
		return (block);
	if (is_grammar_rule(&block, text))
		return (block);
	block.text = protect_malloc(ft_strdup(text));
	if (last_blocktype(blocks) != B_DOUBLE_LESSER)
		expand_environment_variables(&block.text);
	remove_quotes(&block.text);
	block.type = B_TEXT;
	return (block);
}

t_exitcode	cmd_to_blocks(t_list *blocks, const t_list *cmd_split)
{
	size_t	i;
	t_block	block;

	list_init_safe(blocks, sizeof(t_block));
	i = 0;
	while (i < cmd_split->count)
	{
		block = to_block(blocks, *(char **)list_index_unchecked(cmd_split, i));
		if (i == 0 && block.type & B_REDIRECT)
			list_push_safe(blocks, &(t_block){"echo", B_TEXT});
		list_push_safe(blocks, &block);
		i++;
	}
	if (invalid_grammar_rule(blocks) || ambiguous_redirect(blocks, cmd_split))
		return (E_MISUSE_BUILTINS);
	return (E_SUCCESS);
}
