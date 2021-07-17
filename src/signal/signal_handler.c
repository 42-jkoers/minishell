#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

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
