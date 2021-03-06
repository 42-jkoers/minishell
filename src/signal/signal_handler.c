#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

#include "ft_ternary.h"
#include "executable.h"
#include "signal_handler.h"
#include "read_and_execute.h"
#include "utils.h"

// TODO: Look at how you can still use the default event hook
// the rl_done variable is only checked in the event loop
// and the default event hook only returns when a key is pressed
// so setting rl_done to true does not actually return it untill a keypress
// i "solved" this by making it not wait on a keypress
//	but just check as fast as possible
// I'd prefer if i can keep the original behaviour:
//	only updating on key press but add the event when int_handler is called

static int	event_hook(void)
{
	return (0);
}

static void	int_handler(int sig)
{
	(void)sig;
	*controll_c_pressed() |= ter_int(*is_executing_command(),
		pressed_while_executing, pressed);
	rl_done = true;
	return ;
}

static void	quit_handler(int sig)
{
	(void)sig;
}

void	setup_signals(void)
{
	signal(SIGINT, int_handler);
	signal(SIGQUIT, quit_handler);
	rl_event_hook = event_hook;
	rl_catch_signals = 0;
}
