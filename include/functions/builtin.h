#ifndef BUILTIN_H
# define BUILTIN_H

# include "t_builtin_data.h"

# include <sys/types.h>

const t_builtin_exec	*executable_get_builtin_exec(const t_executable
							*executable);

// The whole buildin hack could be completely avoided...
// IF i had access to the mmap function so i could have shared memory :/

// builtin commands

void					*builtin_cd_main(const t_executable *command);
void					builtin_cd_main_cleanup(char *str, pid_t child_pid);
void					builtin_cd_child(char *str);

void					builtin_pwd_child(void *data);

void					builtin_echo_child(const t_executable *data);

void					builtin_env_child(void *data);

void					*builtin_export_main(const t_executable *command);
void					builtin_export_main_cleanup(char *str, pid_t child_pid);
void					builtin_export_child(char *str);

void					*builtin_unset_main(const t_executable *command);
void					builtin_unset_main_cleanup(char *str, pid_t child_pid);
void					builtin_unset_child(char *str);

void					*builtin_exit_main(const t_executable *command);
void					builtin_exit_main_cleanup(char *str, pid_t child_pid);
void					builtin_exit_child(char *str);

#endif