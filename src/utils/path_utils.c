#include "libft.h"

// Joints two paths together, making sure there is one, and only one / in between the two paths
// path_join("~[/]","[/]Desktop") = ~/Desktop
char* path_join(const char* start, const char* end)
{
	int start_len = ft_strlen(start);
	if (end[0] == '/')
		end++;
	if (start_len == 0 || start[start_len - 1] != '/')
		return ft_strjoin_va(3, start, "/", end);
	return ft_strjoin(start, end);
}
