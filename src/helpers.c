#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"

// This file contains functions that have no logical place yet

void	exit_with_error(const char *msg)
{
	printf("%s\n", msg);
	exit(1);
}

char	*ft_strndup_unsafe(const char *str, size_t len)
{
	char	*dup;

	dup = ft_malloc((len + 1) * sizeof(char));
	ft_memcpy(dup, str, len * sizeof(char));
	dup[len] = '\0';
	return (dup);
}
