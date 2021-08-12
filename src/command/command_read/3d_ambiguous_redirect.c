#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include "ft_list.h"
#include "ft_ternary.h"
#include <stdio.h>

static bool	find_next_redirect(size_t *i, const t_list *blocks)
{
	while (*i + 1 < blocks->count)
	{
		(*i)++;
		if (((t_block *)list_index_unchecked(blocks, *i))->type & B_REDIRECT)
			return (true);
	}
	return (false);
}

bool	 ambiguous_redirect(const t_list *blocks, const t_list *split)
{
	size_t	i;
	char	*text;
	char	quote;

	i = -1;
	quote = 0;
	while (find_next_redirect(&i, blocks))
	{
		text = (char *)((t_block *)list_index_unchecked(blocks, i + 1))->text;
		while (*text)
		{
			if (type_quote(*text))
				quote = ter_char(quote == *text, 0, *text);
			if (!quote && ft_isspace(*text))
			{
				printf("minishell: %s: ambiguous redirect\n", (char *)list_index_unchecked(split, i));
				return (true);
			}
			(*text)++;
		}
	}
	return (false);
}
