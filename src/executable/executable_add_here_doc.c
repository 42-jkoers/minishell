#include <stdio.h>
#include "t_executable.h"
#include "t_fd_override.h"
#include "minishell.h"
#include "readline_ext.h"
#include "utils.h"

#include <unistd.h>

static bool	read_here_doc(int fd, const char *end_string)
{
	t_readline_ret_type	ret;
	char				*here_doc;

	while (true)
	{
		ret = readline_ext(">", &here_doc);
		if (ret == NORMAL)
		{
			if (ft_strncmp(here_doc, end_string, ~0) == 0)
				break ;
			stupid_write(fd, here_doc, ft_strlen(here_doc));
			stupid_write(fd, "\n", 1);
		}
		else if (ret == CONTROL_D)
		{
			printf(SHELL": warning: here-document delimited by eof-of-file "
				"(wanted `%s')\n", end_string);
			break ;
		}
		else if (ret == CONTROL_C)
			return (false);
		else
			exit_with_error("What is this ret??");
	}
	return (true);
}

bool	executable_add_here_doc(t_executable *executable, const char *
	end_string)
{
	int	fd[2];

	if (pipe(fd))
		exit_with_error("Pipe failed");
	if (!read_here_doc(fd[1], end_string))
	{
		close(fd[0]);
		close(fd[1]);
		return (false);
	}
	list_push_safe(&executable->child_close_fds, &fd[0]);
	list_push_safe(&executable->main_close_fds, &fd[0]);
	list_push_safe(&executable->fd_overrides, &(t_fd_override){
		.new_fd = fd[0],
		.override_fd = STDIN_FILENO
	});
	close(fd[1]);
	return (true);
}
