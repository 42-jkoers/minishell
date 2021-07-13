#include "t_executable.h"
#include "malloc_wrappers.h"

void	executable_add_arg(t_executable *executable, const char *arg)
{
	list_push_safe(&executable->args, &arg);
}
