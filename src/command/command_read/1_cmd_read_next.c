#include "command_read.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdlib.h>
#include <stdio.h>
#include "readline_ext.h"

// when cmd is "echo > |" this is to handle that error case
static bool	is_valid_redirect(const char *cmd, const char *start_from)
{
	start_from--;
	while (start_from > cmd)
	{
		if (get_grammar_rule_info(start_from).type & B_GRAMMAR_RULE)
			return (false);
		if (!ft_isspace(*start_from))
			return (true);
		start_from--;
	}
	return (false);
}

static bool	ends_with_pipe(const char *cmd)
{
	size_t	i;

	i = ft_strlen(cmd);
	while (i)
	{
		i--;
		if (!ft_isspace(cmd[i]))
			return (cmd[i] == '|' && is_valid_redirect(cmd, cmd + i));
	}
	return (false);
}

// I feel like i over-used tail calls here, well, whatever

// Also frees cmd
static char	*handle_trailing_pipe(char *cmd)
{
	char				*new;
	char				*pipe_read;
	t_readline_ret_type	ret;

	if (!ends_with_pipe(cmd))
		return (cmd);
	ret = readline_ext("> ", &pipe_read);
	if (ret == CONTROL_C)
	{
		free(cmd);
		free(pipe_read);
		return (cmd_read_next());
	}
	else if (ret == CONTROL_D)
	{
		printf("exit\n");
		exit(0);
	}
	new = protect_malloc(ft_strjoin(cmd, pipe_read));
	free(cmd);
	free(pipe_read);
	return (handle_trailing_pipe(new));
}

char	*cmd_read_next(void)
{
	t_readline_ret_type	ret;
	char				*cmd;

	ret = readline_ext("minishell$ ", &cmd);
	if (ret == CONTROL_C)
	{
		free(cmd);
		return (cmd_read_next());
	}
	else if (ret == CONTROL_D)
	{
		printf("exit\n");
		exit(0);
	}
	return (handle_trailing_pipe(cmd));
}
