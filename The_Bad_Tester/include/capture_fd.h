#ifndef CAPTURE_FD_H
# define CAPTURE_FD_H

#include <stdbool.h>

typedef struct s_capture_data
{
	const bool capture_writes;	// if true, it captures writes, if false, it captures reads

	const int captured_fd;
	const int true_fd;

	const int read_end;
	const int write_end;
} t_capture_data;

// Captures a fd
//	When capture_writes == true anything that is written to this FD is captured and for you to read via t_capture_data.read_end
//	When capture_writes = false anything that is read from this FD is reading from t_capture_data.write_end (wich you can write to!)
t_capture_data capture_fd(int fd, bool capture_writes);
// Releases the FD, returning it to its original functionality, but you can still read from t_capture_data.read_end if so desired
void release_fd(t_capture_data* ref);
void free_captured_fd(t_capture_data* ref);

#endif