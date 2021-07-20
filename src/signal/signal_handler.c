#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>

#include "command_read.h"

//if (*get_running_executable() < 0)
// TODO: use rl_done global variable

static void	int_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 1);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
	return ;
}

static void	quit_handler(int sig)
{
	(void)sig;
	return ;
}

void	setup_signals(void)
{
	signal(SIGINT, int_handler);
	signal(SIGQUIT, quit_handler);
}
