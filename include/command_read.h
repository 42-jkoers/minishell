#ifndef COMMAND_READ_H
# define COMMAND_READ_H

# include "ft_list.h"

t_list	command_read(void);
bool	goto_next_split(char **current, char **start, char **end);

#endif
