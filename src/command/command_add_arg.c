#include "t_command.h"

void command_add_arg(t_command* command, const char* arg)
{
	list_push(&command->args, &arg);
}