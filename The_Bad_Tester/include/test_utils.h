#ifndef TEST_UTILS_H
# define TEST_UTILS_H

# include <stdbool.h>

void test_new(char* test_name);

// Probably add varargs with a printf format
void test_ok(char* message);
void test_ko(char* message);
void test_warning(char* message);
void test_sig(char* message, bool good_seg);

char* read_fd_completely(int fd, int* num_chars);

#endif