#include <stddef.h>
#include "libft.h"
#include "minishell.h"

char*** env_ptr()
{
	static char** env = NULL;

	return &env;
}

void env_ptr_copy(char** envp)
{
	size_t size = 0;
	while (envp[size])
		size++;

	char** new_ptr = ft_malloc((size + 1) * sizeof(char*));
	size_t i = 0;
	while (i <= size)
	{
		new_ptr[i] = ft_strdup(envp[i]);
		i++;
	}
	*env_ptr() = new_ptr;
}