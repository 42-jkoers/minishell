#ifndef READLINE_EXT_H
# define READLINE_EXT_H

typedef enum e_readline_ret_type
{
	NORMAL,
	CONTROL_D,
	CONTROL_C
}	t_readline_ret_type;

t_readline_ret_type	readline_ext(const char *promt, char **o_read);

#endif