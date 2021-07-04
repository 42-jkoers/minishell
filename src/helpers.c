#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include "minishell.h"
#include "malloc_wrappers.h"

// This file contains functions that have no logical place yet

void	exit_with_error(const char *msg) \
	__attribute__((noreturn));

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

static void	free_charptr(char **ptr)
{
	free(*ptr);
}

void	list_un_init_charp(t_list *list)
{
	list_un_init(list, (t_free_values)free_charptr);
}
