#ifndef T_EXECUTABLE_H
# define T_EXECUTABLE_H

# include "ft_list.h"

// A executable is a the lowest builting block of a line
// Just contains [echo, hello, "], everything is escaped and elements are just
//a null terminated char*
// Also its file descriptor overrides when using pipes or file inputs/outputs
// And a list of fd's to close, good practice to close unused fd's

typedef struct s_executable
{
	char		*executable_path;
	char *const	*envp;
	t_list		args;
	t_list		fd_overrides;
	t_list		child_close_fds;
	t_list		main_close_fds;
}	t_executable;

#endif
