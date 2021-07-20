#include "executable.h"
#include "t_fd_override.h"
#include <stdio.h>

static void	log_arg(const char **arg)
{
	printf("\t\t%s\n", *arg);
}

static void	log_fd_overrides(const t_fd_override *override)
{
	printf("\t\t%i > %i\n", override->override_fd, override->new_fd);
}

static void	log_fd(const int *fd)
{
	printf("\t\t%i\n", *fd);
}

void	executable_log(const t_executable *executable)
{
	printf("Executable path: %s\n", executable->executable_path);
	printf("\tNum args: %lu\n", executable->args.count);
	list_foreach(&executable->args, (t_foreach_value)log_arg);
	printf("\tNum fd overrides: %lu\n", executable->fd_overrides.count);
	list_foreach(&executable->fd_overrides, (t_foreach_value)log_fd_overrides);
	printf("\tNum child close fd's: %lu\n", executable->child_close_fds.count);
	list_foreach(&executable->child_close_fds, (t_foreach_value)log_fd);
	printf("\tNum main close fd's: %lu\n", executable->main_close_fds.count);
	list_foreach(&executable->main_close_fds, (t_foreach_value)log_fd);
}
