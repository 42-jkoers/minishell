#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

# include <stdbool.h>

typedef enum e_controll_c_pressed_status
{
	f_pressed = 1,
	f_pressed_while_executing = 2,

	pressed = f_pressed,
	pressed_while_executing = f_pressed | f_pressed_while_executing
}	t_controll_c_pressed_status;

void	setup_signals(void);
t_controll_c_pressed_status	*controll_c_pressed(void);

#endif