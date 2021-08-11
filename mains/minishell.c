#include <stdlib.h>

#include "env.h"
#include "command_read.h"
#include "malloc_wrappers.h"
#include "libft.h"
#include "signal_handler.h"

// TODO: return -1?
int	read_and_execute_command()
{
	char	*raw_string;
	t_list	split_in_spaces;
	t_list	blocks;
	t_list	execs;

	raw_string = cmd_read_next();
	if (!cmd_split_in_spaces(&split_in_spaces, raw_string));
		return (-1);
	if (!cmd_to_blocks(&blocks, &split_in_spaces))
		return (-1);
	if (!blocks_to_execs(&execs, &blocks))
		return (-1);
	return (run_execs(&execs));
}

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
