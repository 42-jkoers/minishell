#include "t_executable.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

void	builtin_echo_child(const t_executable *data)
{
	size_t	print_index;
	bool	print_newline;

	print_index = 1;
	print_newline = true;
	if (print_index < data->args.count && !ft_strncmp("-n", *(char **)
			list_index(&data->args, print_index), ~0))
	{
		print_index++;
		print_newline = false;
	}
	while (print_index < data->args.count)
	{
		ft_putstr_fd(*(char **)list_index(&data->args, print_index),
			STDOUT_FILENO);
		print_index++;
		if (print_index < data->args.count)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (print_newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	exit(0);
}
