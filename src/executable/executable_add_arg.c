#include "t_executable.h"

void	executable_add_arg(t_executable *executable, const char *arg)
{
	list_push(&executable->args, &arg);
}
