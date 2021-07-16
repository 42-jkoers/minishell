#include "t_executable.h"
#include "t_fd_override.h"
#include "minishell.h"
#include "malloc_wrappers.h"
#include <stdio.h>
#include <readline/readline.h>

#include <stdlib.h>
#include <unistd.h>

static void	read_here_doc(int fd, const char *end_string)
{
	char	*here_doc;

	while (true)
	{
		here_doc = readline("> ");
		if (!here_doc)
		{
			printf("minishell: warning: here-document delimited by eof-of-file "
				"(wanted %s)\n", end_string);
			break ;
		}
		if (ft_strncmp(here_doc, end_string, ~0) == 0)
			break ;
		write(fd, here_doc, ft_strlen(here_doc));
		write(fd, "\n", 1);
	}
}

void	executable_add_here_doc(t_executable *executable, const char *
	end_string)
{
	int	fd[2];

	if (pipe(fd))
		exit_with_error("Pipe failed");
	read_here_doc(fd[1], end_string);
	list_push_safe(&executable->child_close_fds, &fd[0]);
	list_push_safe(&executable->main_close_fds, &fd[0]);
	list_push_safe(&executable->fd_overrides, &(t_fd_override){
		.new_fd = fd[0],
		.override_fd = STDIN_FILENO
	});
	close(fd[1]);
}
