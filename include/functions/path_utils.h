#ifndef PATH_UTILS_H
# define PATH_UTILS_H

// Joints two paths together, making sure there is one, and only one / in 
//between the two paths
// path_join("~[/]","[/]Desktop") = ~/Desktop
char	*path_join(const char *start, const char *end);

// Simplifies a path when it contains . or ..
void	path_simplify(char *path);

#endif