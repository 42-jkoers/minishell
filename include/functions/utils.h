#ifndef UTILS_H
# define UTILS_H

// Joints two paths together, making sure there is one, and only one / in
//between the two paths
// path_join("~[/]","[/]Desktop") = ~/Desktop
char	*path_join(const char *start, const char *end);

bool	is_valid_env_string(const char *str);
size_t	env_string_length(const char *str);

#endif
