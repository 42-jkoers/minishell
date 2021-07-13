#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include "env.h"

// expect env starting with first char after '$'
static size_t	env_command_length(const char *str)
{
	size_t	i;

	if (*str == '?')
		return (1);
	i = 0;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	return (i);
}

// expect env starting with first char after '$'
static size_t	expand_and_push(char *env, t_list *expanded)
{
	const size_t	env_len = env_command_length(env);
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

// expects a freeable string
void	expand_environment_variables(char **str)
{
	size_t	i;
	t_list	expanded;
	bool	escaped;

	if (!ft_strchr(*str, '$'))
		return ;
	list_init_safe(&expanded, sizeof(char));
	escaped = false;
	i = 0;
	while ((*str)[i])
	{
		if (!escaped && (*str)[i] == '$')
		{
			i += 1 + expand_and_push(*str + i + 1, &expanded);
			escaped = false;
			continue ;
		}
		list_push_safe(&expanded, *str + i);
		escaped = DO_ESCAPE && !escaped && (*str)[i] == '\\';
		i++;
	}
	free(*str);
	list_push_safe(&expanded, "");
	*str = ((char *)expanded.data);
}
