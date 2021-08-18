#ifndef TEST_UTILS_INT_H
# define TEST_UTILS_INT_H

# define CL_COL_OK "\033[0;32m"
# define CL_COL_WARN "\033[1;38;5;208m"
# define CL_COL_KO "\033[0;31m"
# define CL_COL_TEST "\033[0;33m"
# define CL_COL_ERR "\033[0;35m"
# define CL_COL_DEF "\033[0;0m"

// I doubt anyone will print this out, and its on its own file, so it SHOULD be safe.
# define SPLIT_CHAR '\255'
# define SPLIT_STR "\255"

extern int MESSAGE_WRITE_FD;
extern int MESSAGE_READ_FD;

void test_message_init();

#endif