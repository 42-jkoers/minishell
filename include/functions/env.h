#ifndef ENV_H
# define ENV_H

# include "ft_list.h"

t_list		*env_ptr(void);

// copies a env pointer and sets that as the new env_ptr
void		env_copy_ptr(const char **envp);

const char	*env_get(const char *name);
void		env_set(const char *name, const char *new_value);

// Usefull env paths:
// PWD		Current working directory
// PATH		Find programs

#endif