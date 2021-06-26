#ifndef T_FD_OVERRIDE_H
# define T_FD_OVERRIDE_H

// replaces [override_fd] with [new_fd] when running the executable
typedef struct s_fd_override
{
	int	override_fd;
	int	new_fd;
}	t_fd_override;

#endif