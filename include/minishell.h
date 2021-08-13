#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdbool.h>
# include <stddef.h>
# include "libft.h"
# include "ft_list.h"
# define DO_ESCAPE 0
# define EVALUATE_ESCAPED_CHARS 0

// See https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html

__attribute__((noreturn)) \
void exit_with_error(const char *msg);
char	*ft_strndup_unsafe(const char *str, size_t len);
t_list	command_read(void);
void	list_push_safe(t_list *list, const void *value);
void	list_un_init_ptr(t_list *list);
int		ft_strcmp(const char *s1, const char *s2);

typedef enum e_exitcode
{
	E_SUCCESS = 0,
	E_GENERAL_ERR = 1,
	E_MISUSE_BUILTINS = 2,
	E_CANNOT_EXECUTE = 126,
	E_COMMAND_NOT_FOUND = 127,
	E_INVALID_EXIT_ARGUMENT = 128,
	E_FATAL_ERROR_SIGNAL = 128,
	E_STOPPED_BY_CTRL_C = 130,
	E_EXITSTATUS_OUT_OF_RANGE = 255,
}		t_exitcode;

#endif
