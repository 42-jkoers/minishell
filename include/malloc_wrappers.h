#ifndef MALLOC_WRAPPERS_H
# define MALLOC_WRAPPERS_H

# include "ft_list.h"

void	*protect_malloc(void *ptr);
// protect_malloc shorthand
void	*mc(void *ptr);

void	list_init_safe(t_list *list, size_t data_size);
void	list_push_safe(t_list *list, const void *value);
void	*ft_malloc(size_t size);

#endif
