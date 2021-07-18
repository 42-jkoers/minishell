#include "t_executable.h"
#include "t_fd_override.h"
#include "minishell.h"

#include <unistd.h>

// list_push can fail, but ill assume you have changed it so that it just
//exit(1)'s

// thats alot of closing the same fd's :P
void	executable_add_pipe(t_executable *left, t_executable *right)
{
	int	fd[2];

	if (pipe(fd))
		exit_with_error("Pipe failed");
	list_push_safe(&left->child_close_fds, &fd[0]);
	list_push_safe(&left->child_close_fds, &fd[1]);
	list_push_safe(&right->child_close_fds, &fd[0]);
	list_push_safe(&right->child_close_fds, &fd[1]);
	list_push_safe(&right->main_close_fds, &fd[0]);
	list_push_safe(&right->main_close_fds, &fd[1]);
	list_push_safe(&left->fd_overrides, &(t_fd_override){
		.new_fd = fd[1],
		.override_fd = STDOUT_FILENO
	});
	list_push_safe(&right->fd_overrides, &(t_fd_override){
		.new_fd = fd[0],
		.override_fd = STDIN_FILENO
	});
}
