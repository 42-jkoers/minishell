#ifndef UTILS_H
# define UTILS_H

// Joints two paths together, making sure there is one, and only one / in
//between the two paths
// path_join("~[/]","[/]Desktop") = ~/Desktop
char	*path_join(const char *start, const char *end);

bool	is_valid_env_char(char c);
bool	is_valid_env_string(const char *str);

// This is very stupid.
int		stupid_write(int fd, char *str, int len);

#endif
