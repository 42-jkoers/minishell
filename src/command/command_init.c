#include "t_command.h"
#include "t_fd_override.h"
#include "find_executable.h"

void	command_init(t_command *command, char *command_name)
{
	list_init(&command->child_close_fds, sizeof(int));
	list_init(&command->main_close_fds, sizeof(int));
	list_init(&command->fd_overrides, sizeof(t_fd_override));
	list_init(&command->args, sizeof(char *));
	list_push(&command->args, &command_name);
	command->executable_path = find_executable_path(command_name);
}
