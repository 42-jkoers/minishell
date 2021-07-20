#include <unistd.h>

// This is very stupid.

int	stupid_write(int fd, char *str, int len)
{
	return (write(fd, str, len));
}
