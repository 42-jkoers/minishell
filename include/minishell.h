#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdbool.h>
# include <stddef.h>
# include "./libft/include/libft.h"
# include "./libft/include/ft_list.h"

// See https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html

void	*ft_malloc(size_t size);
void	invalid_command(void);
void	internal_error(void);
char	*ft_strndup_unsafe(const char *str, size_t len);
t_list	read_command(void);

#endif
