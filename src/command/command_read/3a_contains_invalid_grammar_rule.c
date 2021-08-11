#include <stdio.h>
#include "command_read.h"

static bool	handle_unexpected_token(
		size_t i,
		const t_block *current,
		const t_list *blocks,
		bool prev_was_grammer_rule)
{
	if (!(current->type & B_GRAMMAR_RULE))
		return (false);
	if (i + 1 != blocks->count && !prev_was_grammer_rule)
		return (false);
	printf("minishell: syntax error near unexpected token ");
	if (i + 1 == blocks->count && !prev_was_grammer_rule)
		printf("`newline'\n");
	else
		printf("`%s'\n", current->text);
	return (true);
}

// return true if there was a invalid grammar rule in the cmd

// returns true:
// foo >>
// foo >> >> bar
// | foo

// returns false (but you expect it to return true)
// >> foo

bool	contains_invalid_grammar_rule(const t_list *blocks)
{
	size_t	i;
	t_block	*current;
	bool	prev_was_grammer_rule;

	prev_was_grammer_rule = true;
	i = 0;
	while (i < blocks->count)
	{
		current = list_index_unchecked(blocks, i);
		if (handle_unexpected_token(i, current, blocks, prev_was_grammer_rule))
			return (true);
		prev_was_grammer_rule = current->type & B_GRAMMAR_RULE;
		i++;
	}
	return (false);
}
