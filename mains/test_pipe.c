#include "command.h"

#include <sys/wait.h>
#include <sys/types.h>

#include <unistd.h>
#include <stdio.h>

int main()
{
	t_command left;
	t_command right;

	command_init(&left, "ls");
	command_init(&right, "grep");

	list_push(&right.args, &"test");

	command_add_pipe(&left, &right);

	// ls | grep test
	pid_t pid_left = command_run(&left);
	pid_t pid_right = command_run(&right);

	int left_status;
	waitpid(pid_left,  &left_status,  0);
	printf("ls exited!\n");

	int right_status;
	waitpid(pid_right, &right_status, 0);
	printf("grep exited!\n");
}