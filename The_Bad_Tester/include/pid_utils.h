#ifndef PID_UTILS_H
# define PID_UTILS_H

# include <sys/types.h>

int get_exit_code(int pid);
void timeout_pid(pid_t pid, float timeout);
pid_t fork_with_timeout(float timeout);

void add_fd_to_close_on_fork(int fd);
void remove_fd_from_close_on_fork(int fd);
void close_all_fds();

#endif