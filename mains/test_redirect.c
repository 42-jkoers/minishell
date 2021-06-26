#include "executable.h"
#include "working_directory.h"
#include "env.h"
#include "libft.h"

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char** argv, const char** envp)
{
	env_ptr_copy(envp);
	(void)argc;
	(void)argv;

	t_executable	executable;
	pid_t		pid;
	int			status;

	// echo 'hello world!' > test_file
	executable_init(&executable, "echo");
	executable_add_arg(&executable, "hello world!");
	if (!executable_add_fd_file_redirect(&executable, STDOUT_FILENO, "test_file", r_write))
	{
		write(STDERR_FILENO, "Error!\ntest_file could not be opened for truncation!\n", 53);
		exit(1);
	}
	pid = executable_run(&executable);
	executable_un_init(&executable);
	waitpid(pid, &status, 0);

	// echo 'hello world!' >> test_file
	executable_init(&executable, "echo");
	executable_add_arg(&executable, "append");
	if (!executable_add_fd_file_redirect(&executable, STDOUT_FILENO, "test_file", r_write | r_append))
	{
		write(STDERR_FILENO,"Error!\ntest_file could not be opened for append!\n", 49);
		exit(1);
	}
	pid = executable_run(&executable);
	executable_un_init(&executable);
	waitpid(pid, &status, 0);

	// Check test_file's contents
	int written_fd = open("test_file", O_RDONLY);
	if (written_fd == -1)
	{
		write(STDOUT_FILENO, "Error!\ncould not read file output!", 75);
		exit(1);
	}
	char buff[128];
	read(written_fd, buff, 128);
	close(written_fd);

	if (ft_strncmp(buff, "hello world!\nappend\n", 128) != 0)
	{
		write(STDOUT_FILENO, "Error!\nFile contents where different than expected, expected:\nhello world!\nappend\n", 83);
		exit(1);
	}
	write(STDOUT_FILENO, "Redirecting output to a file is working!\n", 41);


	// grep hello < test_file
	write(STDOUT_FILENO, "Redirecting a file to input is working if it outputs: \"hello world!\"\n", 70);
	executable_init(&executable, "grep");
	executable_add_arg(&executable, "hello");
	if (!executable_add_fd_file_redirect(&executable, STDIN_FILENO, "test_file", r_read))
	{
		write(STDERR_FILENO,"Error!\ntest_file could not be opened for reading!\n", 50);
		exit(1);
	}
	pid = executable_run(&executable);
	executable_un_init(&executable);
	waitpid(pid, &status, 0);

	//remove("test_file");
}
