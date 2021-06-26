#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// THIS IS STILL IN DEVELOPMENT
// read command from user
// returns malloced char array with command split in spaces according to bash
t_list	command_read(void)
{
	const char		*cmd = readline("minishell$ ");
	t_list			cmd_split;

	cmd_split = get_cmd_split(cmd);
	return (cmd_split);
}
