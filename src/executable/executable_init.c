#include "t_executable.h"
#include "t_fd_override.h"
#include "find_executable.h"
#include "malloc_wrappers.h"

void	executable_init(t_executable *executable, const char *executable_name)
{
	list_init_safe(&executable->child_close_fds, sizeof(int));
	list_init_safe(&executable->main_close_fds, sizeof(int));
	list_init_safe(&executable->fd_overrides, sizeof(t_fd_override));
	list_init_safe(&executable->args, sizeof(const char *));
	list_push_safe(&executable->args, &executable_name);
	executable->executable_path = find_executable_path(executable_name);
}
