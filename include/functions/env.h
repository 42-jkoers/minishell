#ifndef ENV_H
# define ENV_H

char	***env_ptr(void);

// copies a env pointer and sets that as the new env_ptr
void	env_ptr_copy(const char **envp);

const char	*env_get(const char *name);
void	env_set(const char *name, const char *new_value);

// Usefull env paths:
// PWD		Current working directory
// PATH		Find programs

#endif