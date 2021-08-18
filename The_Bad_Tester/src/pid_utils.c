#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

#include <sys/time.h>

int get_exit_code(int status)
{
	if (WIFSIGNALED(status))
		return WTERMSIG(status);
	if (WIFSTOPPED(status))
		return 128 + WSTOPSIG(status);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return -1;	// unknown
}

typedef struct {
	int* ptr;
	int count;
} close_on_forks_list;

static close_on_forks_list* get_close_on_forks()
{
	static close_on_forks_list ptr = {
		.ptr = NULL,
		.count = 0
	};

	return &ptr;
}

void remove_fd_from_close_on_fork(int fd)
{
	close_on_forks_list* ptr = get_close_on_forks();
	for (int i = 0; i < ptr->count; i++)
	{
		if (ptr->ptr[i] == fd)
		{
			ptr->count--;
			ptr->ptr[i] = ptr->ptr[ptr->count];
			return;
		}
	}
	printf("you tried to remove a fd from the close fd's but it wasn't there!\n");
	exit(1);
}

void add_fd_to_close_on_fork(int fd)
{
	close_on_forks_list* ptr = get_close_on_forks();
	ptr->count++;
	ptr->ptr = realloc(ptr->ptr, ptr->count * sizeof(int*));
	ptr->ptr[ptr->count - 1] = fd;
}

void close_all_fds()
{
	close_on_forks_list* ptr = get_close_on_forks();
	for (int i = 0; i < ptr->count; i++)
		close(ptr->ptr[i]);
}

void timeout_pid(pid_t pid, float timeout)
{
	if (timeout > 0)	// negative timeout = no timeout
	{
		// lets fork again, and that one keeps checking this PID
		fflush(NULL);	// Must flush to prevent duplicate messages
		pid_t pid_checker = fork();	// use fork instead of thread just to be safe from this pid being killed
		if (pid_checker == 0)
		{
			close_all_fds();
			// this is now the checker fork
			struct timeval start_time;
			gettimeofday(&start_time, NULL);

			while (1)
			{
				usleep(10000);	// 0.01 seconds
				struct timeval curr_time;
				gettimeofday(&curr_time, NULL);

				float elapsed_time = ((curr_time.tv_sec - start_time.tv_sec) + 0.000001 * (curr_time.tv_usec - start_time.tv_usec));
				if (elapsed_time > timeout)
				{
					// alright, time is up!
					if (kill(pid, SIGKILL) == -1)
						exit(0);	// if it returned -1, we have successfully killed the pid, no use for this fork anymore. exit.
				}
			}
		}
	}
}

pid_t fork_with_timeout(float timeout)
{
	fflush(NULL);	// Must flush to prevent duplicate messages
	pid_t pid = fork();
	if (pid != 0)
		timeout_pid(pid, timeout);
	return pid;
}