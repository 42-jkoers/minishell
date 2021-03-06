#include "libft.h"
#include "env.h"
#include <stdlib.h>
#include "malloc_wrappers.h"

// add a new entry to the env path
// We must keep a null pointer at the end, so insert it at the null pointer

static void	env_add(char *data)
{
	t_list	*env;

	env = env_ptr();
	list_insert_safe(env, env->count - 1, &data);
}

// Returns a reference to where this variable is stored
static char	**get_env_reference(const char *name, size_t name_len)
{
	t_list	*env;
	char	*current;
	size_t	i;

	env = env_ptr();
	i = 0;
	while (i < env->count)
	{
		current = *(char **)list_index(env, i);
		if (current && ft_strncmp(current, name, name_len) == 0
			 && current[name_len] == '=')
			return (list_index(env, i));
		i++;
	}
	return (NULL);
}

const char	*env_get_len(const char *name, size_t name_len)
{
	char	**ref;

	ref = get_env_reference(name, name_len);
	if (!ref)
		return (NULL);
	return (ft_strchr(*ref, '=') + 1);
}

const char	*env_get(const char *name)
{
	return (env_get_len(name, ft_strlen(name)));
}

void	env_set(const char *name, const char *new_value)
{
	char	**ref;

	ref = get_env_reference(name, ft_strlen(name));
	if (ref)
	{
		free(*ref);
		if (new_value == NULL)
			list_remove_safe(env_ptr(),
				list_reverse_index_unchecked(env_ptr(), ref));
		else
			*ref = ft_strjoin_va(3, name, "=", new_value);
	}
	else if (new_value)
		env_add(ft_strjoin_va(3, name, "=", new_value));
}
