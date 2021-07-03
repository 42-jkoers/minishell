#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	escaped_to_ascii(char *c)
{
	const char	typed[] =
	{'a', 'b', 'e', 'f', 'n', 'r', 't', 'v', '?', };
	const char	escaped[] =
	{'\a', '\b', '\e', '\f', '\n', '\r', '\t', '\v', '\?', };
	size_t		i;

	i = 0;
	while (i < sizeof(typed) / sizeof(typed[0]))
	{
		if (*c == typed[i])
		{
			*c = escaped[i];
			return ;
		}
		i++;
	}
}

// eg. "a\ aa" --> "a aa"
//     "\\a" --> "\a"
void	remove_escape_characters(char *cmd)
{
	size_t	i;
	bool	escaped;
	size_t	len;

	len = ft_strlen(cmd);
	escaped = false;
	i = 0;
	while (i < len)
	{
		if (!escaped && cmd[i] == '\\')
		{
			if (EVALUATE_ESCAPED_CHARS)
				escaped_to_ascii(cmd + i + 1);
			ft_memmove(cmd + i, cmd + i + 1, len - i);
			len -= 1;
		}
		escaped = DO_ESCAPE && !escaped && cmd[i] == '\\';
		i++;
	}
}
