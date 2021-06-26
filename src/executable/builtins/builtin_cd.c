#include "t_executable.h"
#include "working_directory.h"
#include "path_utils.h"
#include "libft.h"
#include <unistd.h>
#include <stdlib.h>

void	*builtin_cd_main(const t_executable *command)
{
	bool	success;
	char	*dir;

	if (!(command->args.count >= 1 && command->args.count <= 2))
		return (ft_strdup("bash: cd: too many arguments\n"));
	if (command->args.count == 1)
		working_directory_set("~");
	else
	{
		dir = *(char **)list_index(&command->args, 1);
		success = working_directory_set(path_join(working_directory_get(
						), dir));
		if (!success)
			return (ft_strjoin_va(3, "bash: cd: ", dir, ": No such file or dire"
					"ctory\n"));
	}
	return (NULL);
}

void	builtin_cd_main_cleanup(char *str)
{
	if (str != NULL)
		free(str);
}

void	builtin_cd_child(char *str)
{
	if (str != NULL)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		exit(1);
	}
	exit(0);
}
