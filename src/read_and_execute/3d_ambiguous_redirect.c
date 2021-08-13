#include "read_and_execute.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include "ft_list.h"
#include "ft_ternary.h"
#include <stdio.h>

static bool	find_next_redirect(size_t *i, const t_list *blocks)
{
	while (*i + 2 < blocks->count)
	{
		(*i)++;
		if (((t_block *)list_index(blocks, *i))->type & B_REDIRECT)
			return (true);
	}
	return (false);
}

bool	ambiguous_redirect(const t_list *blocks, const t_list *split)
{
	size_t	i;
	char	*text;
	char	quote;

	i = ~0;
	quote = 0;
	while (find_next_redirect(&i, blocks))
	{
		text = ((t_block *)list_index(blocks, i + 1))->text;
		while (*text)
		{
			if (type_quote(*text))
				quote = ter_char(quote == *text, 0, *text);
			if (!quote && ft_isspace(*text))
			{
				printf(SHELL" %s: ambiguous redirect\n",
					*(char **)list_index(split, i));
				return (true);
			}
			text++;
		}
	}
	return (false);
}
