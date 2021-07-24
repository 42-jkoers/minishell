#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include "env.h"
#include "utils.h"

static size_t	get_env_len(const char *env)
{
	size_t	i;

	if (*env == '?')
		return (1);
	if (ft_isdigit(*env))
		return (1);
	i = 0;
	while (is_valid_env_char(env[i]))
		i++;
	return (i);
}

// expect env starting with first char after '$'
static size_t	expand_and_push(char *env, t_list *expanded)
{
	const size_t	env_len = get_env_len(env);
	char			last_char;
	char			*to_push;
	size_t			i;

	if (env_len == 0)
		return (0);
	last_char = env[env_len];
	env[env_len] = '\0';
	to_push = (char *)env_get(env);
	env[env_len] = last_char;
	if (!to_push)
		return (env_len);
	i = 0;
	while (to_push[i])
	{
		list_push_safe(expanded, &to_push[i]);
		i++;
	}
	return (env_len);
}

// returns mallocd string
char	*expand_environment_variables(const char *str)
{
	size_t	i;
	t_list	expanded;

	if (str[0] && str[ft_strlen(str) - 1] == '$')
		return (protect_malloc(ft_strdup(str)));
	list_init_safe(&expanded, sizeof(char));
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i += 1 + expand_and_push((char *)str + i + 1, &expanded);
			continue ;
		}
		list_push_safe(&expanded, str + i);
		i++;
	}
	list_push_safe(&expanded, "");
	return (((char *)expanded.data));
}
