#include <stddef.h>
#include "libft.h"
#include "env.h"
#include "minishell.h"
#include <stdlib.h>

// add a new entry to the env path
static void env_new(char* data)
{
	char** ptr = *env_ptr();
	size_t prev_size = 0;

	while (ptr[prev_size])
		prev_size++;

	char** new_ptr = ft_malloc((prev_size + 2) * sizeof(char*));
	ft_memcpy(new_ptr, ptr, prev_size * sizeof(char*));
	new_ptr[prev_size] = data;
	new_ptr[prev_size + 1] = NULL;

	free(ptr);
	*env_ptr() = new_ptr;
}

// Returns a reference to where this variable is stored
static char** env_ref(char* name)
{
	char** ptr = *env_ptr();
	size_t strlen = ft_strlen(name);

	while (*ptr)
	{
		if (ft_strncmp(*ptr, name, strlen) == 0 && (*ptr)[strlen] == '=')
			return ptr;
		ptr++;
	}
	return NULL;
}

char* env_get(char* name)
{
	char** ref = env_ref(name);
	if (!ref)
		return NULL;
	return ft_strchr(*ref, '=') + 1;
}

void env_set(char* name, char* new_value)
{
	char** ref = env_ref(name);
	if (ref)
	{
		free(*ref);
		*ref = ft_strjoin_va(3, name, "=", new_value);
	}
	else
		env_new(ft_strjoin_va(3, name, "=", new_value));
}