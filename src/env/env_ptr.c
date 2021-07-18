#include "minishell.h"
#include "malloc_wrappers.h"

#include <stdlib.h>

t_list	*env_ptr(void)
{
	static t_list	env;
	static bool		initialized = false;

	if (!initialized)
	{
		initialized = true;
		list_init_safe(&env, sizeof(char *));
	}
	return (&env);
}

void	env_copy_ptr(const char **envp)
{
	t_list	*env;

	env = env_ptr();
	list_clear(env, free);
	while (envp[0])
	{
		list_push_safe(env, &(char *){ft_strdup(envp[0])});
		envp++;
	}
	list_push_safe(env, &(void *){NULL });
}
