#include "capture_fd.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

t_capture_data capture_fd(int fd, bool capture_writes)
{
	int true_fd = dup(fd);	// create a backup of the original fd, so we can release it

	// Create a pipe so that we will replace the original FD with the write end of this pipe, so when we read at the read end we will read what has been put on [fd]
	int pipefd[2];
	pipe(pipefd);	// pipe2(pipefd, 0); // O_NONBLOCK);
	int read_end = pipefd[0];
	int write_end = pipefd[1];

	fcntl(read_end, F_SETFL, O_NONBLOCK);
	fcntl(write_end, F_SETFL, O_NONBLOCK);

	// replace calls to [fd] with [ret.write_end/ret.read_end]
	if (capture_writes)
		dup2(write_end, fd);
	else
		dup2(read_end, fd);

	return (t_capture_data) {
		.capture_writes = capture_writes,
		.captured_fd = fd,
		.true_fd = true_fd,

		.read_end = read_end,
		.write_end = write_end
	};
}

void release_fd(t_capture_data* ref)
{
	fflush(NULL);	// how to get a FILE* from a fd? dunno, so just flush em all.

	// Select the correct descriptor
	const int* end;
	if (ref->capture_writes)
		end = &ref->write_end;
	else
		end = &ref->read_end;

	dup2(ref->true_fd, ref->captured_fd);

	close(*end);
	*(int*)end = -1;

	close(ref->true_fd);	// close the duplicate we made
	*((int*)&ref->true_fd) = -1;
}

void free_captured_fd(t_capture_data* ref)
{
	const int* end;

	if (ref->capture_writes)
		end = &ref->write_end;
	else
		end = &ref->read_end;
	if (*end != -1)
		release_fd(ref);	// freeing without first releasing

	if (ref->capture_writes)
		end = &ref->read_end;
	else
		end = &ref->write_end;

	close(*end);
	*(int*)end = -1;
}