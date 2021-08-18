#ifndef CATCH_EXCEPTIONS_H
# define CATCH_EXCEPTIONS_H

# include <stdbool.h>

extern bool WOULD_CATCH;

typedef void (*crash_func)(void* arg);

int catch_exceptions(float timeout, crash_func func, void* arg);

#endif