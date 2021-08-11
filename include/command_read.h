#ifndef COMMAND_READ_H
# define COMMAND_READ_H

# include "ft_list.h"
# include <sys/types.h>

// enum as a bit mask
typedef enum e_blocktype
{
	B_ERROR = 1 << 29,
	B_TEXT = 1 << 28,
	B_GRAMMAR_RULE = 1 << 27,
	B_REDIRECT = 1 << 26,
	B_DOLLAR_PREFIX = 1 << 25,
	B_QUOTED = 1 << 24,
	B_CONTAINS_EXPANDED_ENV = 1 << 23,
	B_NO_CLOSING_QUOTE = B_ERROR | 1 << 0,
	B_END = 1 << 1,
	B_NORMAL = B_TEXT | 1 << 2,
	B_SINGLE_QUOTE = B_TEXT | B_QUOTED | 1 << 3,
	B_DOUBLE_QUOTE = B_TEXT | B_QUOTED | 1 << 4,
	B_DOUBLE_GREATER = B_GRAMMAR_RULE | B_REDIRECT | 1 << 5,
	B_DOUBLE_LESSER = B_GRAMMAR_RULE | B_REDIRECT | 1 << 6,
	B_GREATER = B_GRAMMAR_RULE | B_REDIRECT | 1 << 7,
	B_LESSER = B_GRAMMAR_RULE | B_REDIRECT | 1 << 8,
	B_PIPE = B_GRAMMAR_RULE | 1 << 10,
	B_ENV_DECLARATION = 1 << 11,
	B_SINGLE_QUOTE_ONLY = 1 << 10,
}			t_blocktype;

typedef struct s_block
{
	char		*text;
	t_blocktype	type;
}		t_block;

typedef enum e_status
{
	SUCCESS,
	FAIL,
	DONE,
}		t_status;

typedef struct s_grammarinfo
{
	t_blocktype	type;
	size_t		len;
}				t_grammarinfo;

// IMPORTANT ORDER
char			*cmd_read_next(void);
bool			cmd_split_in_spaces(t_list *split, const char *cmd);
bool			cmd_to_blocks(t_list *blocks, const t_list *cmd_split);
bool			blocks_to_execs(t_list *execs, const t_list *blocks);
int				run_execs(const t_list *execs);


void			command_read_destroy(t_list *cmd);
// Negative if nothing is running
pid_t			*get_running_executable(void);


// =============== PRIVATE HELPERS ===============

t_blocktype		type_quote(char c);
t_grammarinfo	get_grammar_rule_info(const char *str);

t_status		goto_next_split(char **start, char **end);

void			expand_environment_variables(char **str);
void			remove_quotes(char **str);

void			push_execs(t_list *execs, const t_list *cmd);

#endif
