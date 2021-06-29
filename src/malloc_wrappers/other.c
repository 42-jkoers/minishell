#include "malloc_wrappers.h"
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>

void	*malloc_check(void *ptr)
{
	if (ptr == NULL)
		exit_with_error("malloc");
	return (ptr);
}

void	*mc(void *ptr)
{
	return (malloc_check(ptr));
}

void	*ft_malloc(size_t size)
{
	return (malloc_check(malloc(size)));
}
