#ifndef COMMAND_H
# define COMMAND_H

# include "t_command.h"
# include <sys/types.h>

// Returns the procid of the new process, -1 on error
pid_t	command_run(const t_command *command);

typedef enum e_file_redirect_mode
{
	r_read = 0,
	r_write = 1,
	r_append = 2,
}	t_file_redirect_mode;

// Handles things like: left | right
void	command_add_pipe(t_command *left, t_command *right);
// Handles things like: [n]>&[n]
void	command_add_fd_redirect(t_command *command, int base_fd, int new_fd);
// Handles things like [n]>test_file, [n]>>test_file and [n]<test_file
// returns false when the file could not be opened (eg: read non-existent file)
bool	command_add_fd_file_redirect(t_command *command, int base_fd, char
			*file_path, t_file_redirect_mode redirect_mode);

// Executable path can still be null when you have a bad exec, make sure to
//check it!
// And the args will only contain [exec], so add the rest too!
void	command_init(t_command *command, char *exec);
void	command_un_init(t_command *command);

void	command_add_arg(t_command *command, const char *arg);

#endif