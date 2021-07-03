#ifndef COMMAND_READ_H
# define COMMAND_READ_H

# include "ft_list.h"

typedef enum e_blocktype
{
	NOTFOUND,
	NO_CLOSING_QUOTE,
	NORMAL,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	GRAMMAR_RULE,
}			t_blocktype;

t_list		command_read(void);
t_blocktype	goto_next_split(char **current, char **start, char **end);
void		expand_environment_variables(char **str);

t_blocktype	handle_quoted_block(char **current, char **start, char **end,
				t_blocktype blocktype);

#endif
