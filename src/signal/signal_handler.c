#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

#include "command_read.h"
#include "utils.h"

// TODO: Look at how you can still use the default event hook
// the rl_done variable is only checked in the event loop
// and the default event hook only returns when a key is pressed
// so setting rl_done to true does not actually return it untill a keypress
// i "solved" this by making it not wait on a keypress
//	but just check as fast as possible
// I'd prefer if i can keep the original behaviour:
//	only updating on key press but add the event when int_handler is called

bool	*controll_c_pressed(void)
{
	static bool	c_pressed = false;

	return (&c_pressed);
}

static int	event_hook(void)
{
	return (0);
}

static void	int_handler(int sig)
{
	(void)sig;
	*controll_c_pressed() = true;
	rl_done = true;
	return ;
}

static void	quit_handler(int sig)
{
	(void)sig;
	stupid_write(STDIN_FILENO, "\b \b\b \b", 6);
	return ;
}

void	setup_signals(void)
{
	signal(SIGINT, int_handler);
	signal(SIGQUIT, quit_handler);
	rl_event_hook = event_hook;
}
