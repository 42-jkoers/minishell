#include <stdlib.h>

#include "env.h"
#include "read_and_execute.h"
#include "malloc_wrappers.h"
#include "libft.h"
#include "signal_handler.h"

// TODO: exit code??
int	main(int argc, char **argv, const char **envp)
{
	char	*exitcode_str;

	env_copy_ptr(envp);
	setup_signals();
	(void)argc;
	(void)argv;
	env_set("?", "0");
	while (true)
	{
		exitcode_str = protect_malloc(ft_itoa(read_and_execute_command()));
		env_set("?", exitcode_str);
		free(exitcode_str);
	}
}
