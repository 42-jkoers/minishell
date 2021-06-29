#ifndef MALLOC_WRAPPERS_H
# define MALLOC_WRAPPERS_H

# include "ft_list.h"

void	*malloc_check(void *ptr);
// malloc_check shorthand
void	*mc(void *ptr);

void	list_init_safe(t_list *list, size_t data_size);
void	list_push_safe(t_list *list, const void *value);
void	*ft_malloc(size_t size);

#endif
