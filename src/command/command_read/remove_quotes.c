#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include "libft.h"
#include "env.h"
#include "utils.h"
#include <stdio.h>
#include "ft_ternary.h"

void	loop_chars(t_list *text, const char *start)
{
	char	quote;
	char	*current;

	current = (char *)start;
	quote = ter_char(type_quote(*start), *start, 0);
	if (type_quote(*current))
		current++;
	while (*current)
	{
		if (!(quote && type_quote(*current)))
			list_push_safe(text, current);
		if (type_quote(*current))
			quote = ter_char(quote == *current, 0, *current);
		current++;
	}
}

// expects mallocd string
void	remove_quotes(char **str)
{
	t_list	text;

	list_init_safe(&text, sizeof(char));
	loop_chars(&text, *str);
	list_push_safe(&text, "");
	free(*str);
	*str = text.data;
}
