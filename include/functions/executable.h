#ifndef EXECUTABLE_H
# define EXECUTABLE_H

# include "t_executable.h"
# include <sys/types.h>

// Returns the procid of the new process, -1 on error
pid_t	executable_run(const t_executable *executable);

typedef enum e_file_redirect_mode
{
	r_read = 0,
	r_write = 1,
	r_append = 2,
}	t_file_redirect_mode;

// Handles things like: left | right
void	executable_add_pipe(t_executable *left, t_executable *right);
// Handles things like: [n]>&[n]
void	executable_add_fd_redirect(t_executable *executable, int base_fd,
			int new_fd);
// Handles things like [n]>test_file, [n]>>test_file and [n]<test_file
// returns false when the file could not be opened (eg: read non-existent file)
bool	executable_add_fd_file_redirect(t_executable *executable, int base_fd,
			char
			*file_path, t_file_redirect_mode redirect_mode);

// Handless things like <<[end_string]
// Returns false when ^C was pressed while writing
bool	executable_add_here_doc(t_executable *executable, const char
			*end_string);

// Executable path can still be null when you have a bad exec, make sure to
//check it!
// And the args will only contain [exec], so add the rest too!
void	executable_init(t_executable *executable, const char *exec);
void	executable_un_init(t_executable *executable);

void	executable_add_arg(t_executable *executable, const char *arg);

bool	*is_executing_command(void);
int		executable_run_all(t_list *executables);

void	executable_log(const t_executable *executable);

#endif