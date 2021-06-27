#include "libft.h"
#include "t_executable.h"
#include "builtin.h"

static void	*builtin_null_main(const t_executable *command)
{
	(void)command;
	return (NULL);
}

static void	builtin_null_main_cleanup(void *data)
{
	(void)data;
}

static const t_builtin_data	*get_builtin_data(void)
{
	static const t_builtin_data	builtin_data[] = {
		(t_builtin_data){
			.name = "cd",
			.exec_func.main_func = builtin_cd_main,
			.exec_func.main_cleanup_func = (t_builtin_main_cleanup_func)
				builtin_cd_main_cleanup,
			.exec_func.child_func = (t_builtin_child_func)builtin_cd_child
		},
		(t_builtin_data){
			.name = "pwd",
			.exec_func.main_func = builtin_null_main,
			.exec_func.main_cleanup_func = builtin_null_main_cleanup,
			.exec_func.child_func = (t_builtin_child_func)builtin_pwd_child
		},
		(t_builtin_data){.name = NULL }
	};

	return (builtin_data);
}

const t_builtin_exec	*executable_get_builtin_exec(const t_executable *
	executable)
{
	const char				*exec_name;
	const t_builtin_data	*builtin_data = get_builtin_data();

	exec_name = *((const char **)list_index(&executable->args, 0));
	while (builtin_data->name)
	{
		if (ft_strncmp(exec_name, builtin_data->name, ~0) == 0)
			return (&builtin_data->exec_func);
		builtin_data++;
	}
	return (NULL);
}
