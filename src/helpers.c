#include "libft/include/libft.h"
#include <stdio.h>
#include <stdlib.h>

void	*ft_malloc(size_t size)
{
	void	*p;

	p = malloc(size);
	if (p == NULL)
	{
		printf("Malloc failed\n");
		exit(1);
	}
	return (p);
}

void	invalid_command(void)
{
	printf("Invalid command\n");
	exit(1);
}

void	internal_error(void)
{
	printf("Internal error");
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
