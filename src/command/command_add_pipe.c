#include "t_command.h"
#include "t_fd_override.h"

#include <stdlib.h>
#include <unistd.h>

// list_push can fail, but ill assume you have changed it so that it just exit(1)'s

void command_add_pipe(t_command* left, t_command* right)
{
	int fd[2];

	if (pipe(fd))
		exit(1);

	list_push(&left->child_close_fds, &fd[0]);
	list_push(&left->child_close_fds, &fd[1]);
	list_push(&right->child_close_fds, &fd[0]);
	list_push(&right->child_close_fds, &fd[1]);

	list_push(&right->main_close_fds, &fd[0]);
	list_push(&right->main_close_fds, &fd[1]);

	list_push(&left->fd_overrides, &(t_fd_override){
		.new_fd = fd[1],
		.override_fd = STDOUT_FILENO
	});
	list_push(&right->fd_overrides, &(t_fd_override){
		.new_fd = fd[0],
		.override_fd = STDIN_FILENO
	});
}