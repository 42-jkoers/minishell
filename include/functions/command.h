#ifndef COMMAND_H
# define COMMAND_H

# include "t_command.h"
# include <sys/types.h>

// Returns the procid of the new process, -1 on error
pid_t	command_run(const t_command *command);

// makes it so it does this: left | right
void	command_add_pipe(t_command *left, t_command *right);

// Executable path can still be null when you have a bad exec, make sure to
//check it!
// And the args will only contain [exec], so add the rest too!
void	command_init(t_command *command, char *exec, char *const *envp);

void	command_add_arg(t_command *command, const char *arg);

#endif