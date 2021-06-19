#ifndef COMMAND_H
# define COMMAND_H

#include "t_command.h"
#include <sys/types.h>

// Returns the procid of the new process, -1 on error
pid_t run_command(const t_command* command);

#endif