#include "readline_ext.h"
#include "signal_handler.h"

#include <readline/readline.h>
#include <readline/readline.h>

t_readline_ret_type	readline_ext(const char *promt, char **o_read)
{
	*controll_c_pressed() = false;
	*o_read = readline(promt);
	if (*o_read == NULL)
		return (CONTROL_D);
	if (*controll_c_pressed())
		return (CONTROL_C);
	return (NORMAL);
}
