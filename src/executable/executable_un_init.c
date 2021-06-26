#include "t_executable.h"
#include <stdlib.h>

void	executable_un_init(t_executable *executable)
{
	list_un_init(&executable->child_close_fds, NULL);
	list_un_init(&executable->main_close_fds, NULL);
	list_un_init(&executable->fd_overrides, NULL);
	list_un_init(&executable->args, NULL);
	free(executable->executable_path);
}
