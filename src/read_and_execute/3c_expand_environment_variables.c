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
	char			*to_push;
	size_t			i;

	if (env_len == 0)
	{
		list_push_safe(expanded, "$");
		return (0);
	}
	to_push = (char *)env_get_len(env, env_len);
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

// expects mallocd string
void	expand_environment_variables(char **str)
{
	size_t	i;
	t_list	expanded;
	bool	quoted;

	if (!ft_strncmp(*str, "$", ~0))
		return ;
	list_init_safe(&expanded, sizeof(char));
	i = 0;
	quoted = false;
	while ((*str)[i])
	{
		if ((*str)[i] == '\'')
			quoted = !quoted;
		if ((*str)[i] == '$' && !quoted)
			i += 1 + expand_and_push(*str + i + 1, &expanded);
		else
		{
			list_push_safe(&expanded, *str + i);
			i++;
		}
	}
	list_push_safe(&expanded, "");
	free(*str);
	*str = expanded.data;
}
