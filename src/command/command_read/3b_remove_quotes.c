#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include "libft.h"
#include "env.h"
#include "utils.h"
#include <stdio.h>
#include "ft_ternary.h"

void	remove_quotes(char **str)
{
	char	*src;
	char	*dst;
	char	quote;

	src = *str;
	dst = *str;
	quote = 0;
	while (*src && *dst)
	{
		if ((type_quote(*src) && !quote) || quote == *src)
		{
			quote = ter_char(quote == *src, 0, *src);
			src++;
			continue ;
		}
		*dst = *src;
		dst++;
		src++;
	}
	*dst = 0;
}
