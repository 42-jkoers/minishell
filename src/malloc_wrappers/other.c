#include "malloc_wrappers.h"
#include "minishell.h"

void	*ft_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
		exit_with_error("malloc");
	return (p);
}
