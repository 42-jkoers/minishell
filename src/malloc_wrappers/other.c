#include "malloc_wrappers.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	*protect_malloc(void *ptr)
{
	if (ptr == NULL)
		exit_with_error("malloc");
	return (ptr);
}

void	*mc(void *ptr)
{
	return (protect_malloc(ptr));
}

void	*ft_malloc(size_t size)
{
	return (protect_malloc(malloc(size)));
}
