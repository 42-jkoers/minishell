#include "libft.h"

static bool	is_valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

// returns -1 if not environment variable string
size_t	env_string_length(const char *str)
{
	size_t	i;

	if (ft_strncmp(str, "?", ~0))
		return (1);
	if (ft_isdigit(*str))
		return (0);
	i = 0;
	while (is_valid_env_char(str[i]))
		i++;
	return (i);
}

bool	is_valid_env_string(const char *str)
{
	return (env_string_length(str));
}
