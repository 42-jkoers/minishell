#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdbool.h>
# include <stddef.h>
# include "libft.h"
# include "ft_list.h"

// See https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html

void	exit_with_error(const char *msg);
char	*ft_strndup_unsafe(const char *str, size_t len);
t_list	command_read(void);
void	list_push_safe(t_list *list, const void *value);

#endif
