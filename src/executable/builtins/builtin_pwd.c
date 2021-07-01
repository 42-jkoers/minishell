#include "t_executable.h"
#include "working_directory.h"
#include "utils.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

void	builtin_pwd_child(void *data)
{
	(void)data;
	ft_putstr_fd(working_directory_get(), STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	exit(0);
}
