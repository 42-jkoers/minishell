#ifndef PATH_UTILS_H
# define PATH_UTILS_H

char* get_basename(char* path);

// Does not malloc, simply returns pointer inside path given
char* get_filename(char* path);
char* path_join(char* start, char* end);

// Converts a relative path from the JTester2.0 dir to a absolute path
char* path_relative(char* start);

#endif