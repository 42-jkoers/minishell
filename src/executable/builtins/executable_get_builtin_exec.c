#include "libft.h"
#include "t_executable.h"
#include "builtin.h"

static void	*builtin_null_main(const t_executable *executable)
{
	return ((void *)executable);
}

static void	builtin_null_main_cleanup(void *data, pid_t child_pid)
{
	(void)data;
	(void)child_pid;
}

// Happy me, i can use the one global variable for this stuff
//	no need to wory about the 25 line rule when i can use globals!

static const t_builtin_data	g_builtin_data[] = {
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
	(t_builtin_data){
		.name = "echo",
		.exec_func.main_func = builtin_null_main,
		.exec_func.main_cleanup_func = builtin_null_main_cleanup,
		.exec_func.child_func = (t_builtin_child_func)builtin_echo_child
	},
	(t_builtin_data){
		.name = "env",
		.exec_func.main_func = builtin_null_main,
		.exec_func.main_cleanup_func = builtin_null_main_cleanup,
		.exec_func.child_func = (t_builtin_child_func)builtin_env_child
	},
	(t_builtin_data){
		.name = "unset",
		.exec_func.main_func = builtin_unset_main,
		.exec_func.main_cleanup_func = (t_builtin_main_cleanup_func)builtin_unset_main_cleanup,
		.exec_func.child_func = (t_builtin_child_func)builtin_unset_child
	},
	(t_builtin_data){
		.name = "exit",
		.exec_func.main_func = builtin_exit_main,
		.exec_func.main_cleanup_func = (t_builtin_main_cleanup_func)builtin_exit_main_cleanup,
		.exec_func.child_func = (t_builtin_child_func)builtin_exit_child
	},
	(t_builtin_data){
		.name = "export",
		.exec_func.main_func = builtin_export_main,
		.exec_func.main_cleanup_func = (t_builtin_main_cleanup_func)builtin_export_main_cleanup,
		.exec_func.child_func = (t_builtin_child_func)builtin_export_child
	},
	(t_builtin_data){.name = NULL }
};

const t_builtin_exec	*executable_get_builtin_exec(const t_executable *
	executable)
{
	const char	*exec_name;
	size_t		current_index;

	exec_name = *((const char **)list_index(&executable->args, 0));
	current_index = 0;
	while (g_builtin_data[current_index].name)
	{
		if (ft_strncmp(exec_name, g_builtin_data[current_index].name, ~0) == 0)
			return (&g_builtin_data[current_index].exec_func);
		current_index++;
	}
	return (NULL);
}
