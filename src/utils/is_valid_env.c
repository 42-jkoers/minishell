#include "libft.h"

bool	is_valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	is_valid_env_string(char *str)
{
	while (*str)
	{
		if (!is_valid_env_char(*str))
			return (false);
		str++;
	}
	return (true);
}
