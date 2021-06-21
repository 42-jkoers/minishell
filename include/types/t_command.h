#ifndef T_COMMAND_H
# define T_COMMAND_H

# include "ft_list.h"

// A command is a the lowest building block of a line
// Just contains [echo, hello, "], everything is escaped and elements are just 
//a null terminated char*

typedef struct s_command
{
	char	*executable_path;
	t_list	args;
	t_list	fd_overrides;
	t_list	child_close_fds;
	t_list	main_close_fds;
}	t_command;

#endif