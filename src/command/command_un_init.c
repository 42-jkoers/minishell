#include "t_command.h"
#include <stdlib.h>

void	command_un_init(t_command *command)
{
	list_un_init(&command->child_close_fds, NULL);
	list_un_init(&command->main_close_fds, NULL);
	list_un_init(&command->fd_overrides, NULL);
	list_un_init(&command->args, NULL);
	free(command->executable_path);
}