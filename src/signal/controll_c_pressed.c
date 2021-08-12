#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include <unistd.h>

#include "ft_ternary.h"
#include "executable.h"
#include "signal_handler.h"
#include "command_read.h"
#include "utils.h"

t_controll_c_pressed_status	*controll_c_pressed(void)
{
	static t_controll_c_pressed_status	c_pressed = false;

	return (&c_pressed);
}
