#include "libft.h"

bool	is_valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	is_valid_env_string(const char *str)
{
	size_t	i;

	if (ft_isdigit(*str))
		return (false);
	i = 0;
	while (is_valid_env_char(str[i]))
		i++;
	return (str[i] == '\0');
}
