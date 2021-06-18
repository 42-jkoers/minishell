/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_parse_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jsimonis <jsimonis@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/01/26 16:58:07 by jsimonis      #+#    #+#                 */
/*   Updated: 2021/06/16 18:27:54 by jsimonis      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_parse_utils.h"
#include "ft_error.h"
#include "libft.h"
#include "ft_ternary.h"

// When we read a overflowing, its nice to read all the digits

static void	skip_digits(const char *str, int *current)
{
	while (ft_isdigit(str[*current]))
		(*current)++;
}

// If you are complaining about the ter_int, well...
// i can replace it with this beautiful piece of code:
//	((~0) >> 1) + is_negative

bool	read_int(const char *str, int *current, int *value)
{
	int				start;
	bool			is_negative;
	unsigned long	long_value;

	start = *current;
	long_value = 0;
	is_negative = skip_char(str, current, '-');
	while (ft_isdigit(str[*current]))
	{
		long_value = long_value * 10 + str[*current] - '0';
		if (long_value > 2147483647 && !(
				is_negative && long_value == 2147483648))
		{
			set_error("Value out of range!", false);
			*value = ter_int(is_negative, -2147483648, 2147483647);
			skip_digits(str, current);
			return (false);
		}
		(*current)++;
	}
	*value = ter_int(is_negative, -long_value, long_value);
	return (start != *current);
}

bool	read_float(const char *str, int *current, float *value)
{
	int		start;
	bool	is_negative;
	float	worth;

	start = *current;
	*value = 0;
	is_negative = skip_char(str, current, '-');
	while (ft_isdigit(str[*current]))
	{
		*value = *value * 10 + str[*current] - '0';
		(*current)++;
	}
	if (skip_char(str, current, '.'))
	{
		worth = 0.1f;
		while (ft_isdigit(str[*current]))
		{
			*value += (str[*current] - '0') * worth;
			worth /= 10;
			(*current)++;
		}
	}
	if (is_negative)
		*value = - *value;
	return (start != *current);
}

bool	read_vec3(const char *str, int *current, char seperator, t_vec3 *o_vec3)
{
	if (!read_float(str, current, &o_vec3->x) || !skip_char(str, current,
			seperator))
		return (false);
	if (!read_float(str, current, &o_vec3->y) || !skip_char(str, current,
			seperator))
		return (false);
	if (!read_float(str, current, &o_vec3->z))
		return (false);
	return (true);
}

bool	read_vec3_unit(const char *str, int *current, char seperator,
	t_vec3 *o_vec3)
{
	if (!read_vec3(str, current, seperator, o_vec3))
		return (false);
	if (vec3_magnitude_sqr(o_vec3) < 0.001)
		return (false);
	vec3_normalize(o_vec3, o_vec3);
	return (true);
}
