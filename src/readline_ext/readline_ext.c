#include "readline_ext.h"
#include "signal_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include <readline/readline.h>
#include <readline/history.h>

//printf("got line: %s and c_pressed: %i\n", *o_read, *controll_c_pressed());

t_readline_ret_type	readline_ext(const char *promt, char **o_read)
{
	if (*controll_c_pressed() & f_pressed_while_executing)
		printf("\n");
	*controll_c_pressed() = false;
	*o_read = readline(promt);
	if (*o_read == NULL)
		return (CONTROL_D);
	if (*controll_c_pressed() & f_pressed)
		return (CONTROL_C);
	return (NORMAL);
}
