#ifndef BUILTIN_H
# define BUILTIN_H

# include "t_builtin_data.h"

const t_builtin_exec	*executable_get_builtin_exec(const t_executable
							*executable);

// builtin commands

void					*builtin_cd_main(const t_executable *command);
void					builtin_cd_main_cleanup(char *str);
void					builtin_cd_child(char *str);

void					builtin_pwd_child(void *data);

#endif