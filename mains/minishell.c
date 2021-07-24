#include <stdlib.h>

#include "env.h"
#include "command_read.h"
#include "malloc_wrappers.h"
#include "libft.h"
#include "signal_handler.h"

// TODO: exit code??
int	main(int argc, char **argv, const char **envp)
{
	t_list	cmd;
	char	*exitcode_str;

	env_copy_ptr(envp);
	setup_signals();
	(void)argc;
	(void)argv;
	env_set("?", "0");
	while (true)
	{
		cmd = command_read();
		exitcode_str = protect_malloc(ft_itoa(run_command_as_executable(&cmd)));
		env_set("?", exitcode_str);
		free(exitcode_str);
		command_read_destroy(&cmd);
	}
}
