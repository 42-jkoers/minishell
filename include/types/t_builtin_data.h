#ifndef T_BUILTIN_DATA_H
# define T_BUILTIN_DATA_H

# include "t_executable.h"

# include <sys/types.h>

typedef void*	(*t_builtin_main_func)(const t_executable* command);
typedef void	(*t_builtin_main_cleanup_func)(void* data, pid_t child_pid);
typedef void	(*t_builtin_child_func)(void* data);

typedef struct s_builtin_exec
{
	t_builtin_main_func			main_func;
	t_builtin_main_cleanup_func	main_cleanup_func;
	t_builtin_child_func		child_func;
}	t_builtin_exec;

typedef struct s_builtin_data
{
	const char				*name;
	const t_builtin_exec	exec_func;
}	t_builtin_data;

#endif